#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

int main() {
    char fileName[BUFFER_SIZE];
    char userData[BUFFER_SIZE];
    int fileDescriptor;

    // Kullanıcıdan dosya adı al
    printf("Oluşturulacak dosyanın adını girin: ");
    if (fgets(fileName, sizeof(fileName), stdin) == NULL) {
        perror("Dosya adı okunamadı");
        return 1;
    }
    // '\n' karakterini kaldır
    fileName[strcspn(fileName, "\n")] = 0;

    // Kullanıcıdan yazılacak veri al
    printf("Dosyaya yazılacak veriyi girin: ");
    if (fgets(userData, sizeof(userData), stdin) == NULL) {
        perror("Veri okunamadı");
        return 1;
    }

    // '\n' karakterini kaldır
    userData[strcspn(userData, "\n")] = 0;

    // Dosyayı oluştur (veya varsa üzerine yaz)
    fileDescriptor = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fileDescriptor < 0) {
        perror("Dosya oluşturulamadı");
        return 1;
    }

    // Veriyi dosyaya yaz
    ssize_t bytesWritten = write(fileDescriptor, userData, strlen(userData));
    if (bytesWritten < 0) {
        perror("Dosyaya yazılamadı");
        close(fileDescriptor);
        return 1;
    }

    printf("Veri başarıyla dosyaya yazıldı.\n");

    // Dosyayı kapat
    close(fileDescriptor);
    return 0;
}
