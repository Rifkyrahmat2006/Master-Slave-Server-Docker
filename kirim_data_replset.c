#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

#include <bson/bson.h>
#include <mongoc/mongoc.h>

#define MAX_JSON_SIZE 4096

int main(int argc, char *argv[]) {
    mongoc_client_t *client = NULL;
    mongoc_collection_t *collection = NULL;
    mongoc_uri_t *uri = NULL;
    bson_error_t error;
    bson_t *doc = NULL;
    char json_buffer[MAX_JSON_SIZE];
    FILE *fp;
    size_t len;

    if (argc != 2) {
        fprintf(stderr, "Penggunaan: %s <path_ke_file_json>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *json_file_path = argv[1];

    mongoc_init();

    // --- URI DISESUAIKAN UNTUK REPLICA SET ANDA ---
    const char *uri_string = "mongodb://GANTI_IP_PRIMARY/MASTER:27017,GANTI_IP_SECONDARY/SLAVE:27017/?replicaSet=rs0&readPreference=primary";
    // Jika perlu autentikasi:
    // const char *uri_string = "mongodb://username:password@GANTI_IP_PRIMARY/MASTER:27017,GANTI_IP_SECONDARY/SLAVE:27017/?replicaSet=rs0&readPreference=primary";
    // -------------------------------------------------

    printf("Mencoba koneksi dengan URI: %s\n", uri_string);

    uri = mongoc_uri_new_with_error(uri_string, &error);
    if (!uri) {
        fprintf(stderr, "Gagal mem-parse URI '%s': %s\n", uri_string, error.message);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    client = mongoc_client_new_from_uri(uri);
    if (!client) {
        fprintf(stderr, "Gagal membuat client MongoDB dari URI.\n");
        mongoc_uri_destroy(uri);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }
    mongoc_uri_destroy(uri);
    uri = NULL;

    mongoc_client_set_appname(client, "c-sender-replset-rifky");

    fp = fopen(json_file_path, "rb");
    if (!fp) {
        fprintf(stderr, "Gagal membuka file JSON '%s': %s\n", json_file_path, strerror(errno));
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }
    len = fread(json_buffer, 1, sizeof(json_buffer) - 1, fp);
    if (ferror(fp)) {
        fprintf(stderr, "Gagal membaca file JSON '%s'\n", json_file_path);
        fclose(fp);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }
    json_buffer[len] = '\0';
    fclose(fp);

      // --- GANTI NAMA DATABASE DAN COLLECTION DI BAWAH INI ---
    const char *db_name = "NAMA_DATABASE_ANDA";
    const char *collection_name = "NAMA_COLLECTION_ANDA";
      // Contoh:
      // const char *db_name = "db_replikasi_saya";
      // const char *collection_name = "data_sensor_replikasi";
      // ------------------------------------------------------

    collection = mongoc_client_get_collection(client, db_name, collection_name);

    doc = bson_new_from_json((const uint8_t *)json_buffer, -1, &error);
    if (!doc) {
        fprintf(stderr, "Gagal mem-parse JSON menjadi BSON: %s\n", error.message);
        mongoc_collection_destroy(collection);
        mongoc_client_destroy(client);
        mongoc_cleanup();
        return EXIT_FAILURE;
    }

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "Gagal memasukkan dokumen: %s\n", error.message);
    } else {
        printf("Dokumen dari file '%s' berhasil dimasukkan ke database '%s', collection '%s'!\n",
               json_file_path, db_name, collection_name);
    }

    bson_destroy(doc);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return EXIT_SUCCESS;
}
