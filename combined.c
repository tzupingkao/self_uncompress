#include <stdio.h>
#include <stdlib.h>

void extract_zip(FILE *combined_file, long zip_start) {
    // 移動到壓縮檔開始位置
    fseek(combined_file, zip_start, SEEK_SET);

    // 創建一個輸出文件來存儲解壓縮的內容
    FILE *zip_file = fopen("data.zip", "wb");
    if (zip_file == NULL) {
        perror("無法創建壓縮檔");
        exit(EXIT_FAILURE);
    }

    // 從壓縮檔位置開始複製內容
    int ch;
    while ((ch = fgetc(combined_file)) != EOF) {
        fputc(ch, zip_file);
    }

    fclose(zip_file);
}

long find_zip_start(FILE *file) {
    // 將檔案指針移動到文件的結尾
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    // 搜索 `.zip` 檔案的簽名
    long pos = file_size - 22; // 略過可能的結尾數據
    while (pos > 0) {
        fseek(file, pos, SEEK_SET);

        // 讀取4個位元組，判斷是否為ZIP標誌
        if (fgetc(file) == 0x50 && fgetc(file) == 0x4B && fgetc(file) == 0x03 && fgetc(file) == 0x04) {
            return pos;
        }
        pos--;
    }
    return -1; // 未找到
}

int main() {
    // 打開拼接的執行檔
    FILE *combined_file = fopen("combined.exe", "rb");
    if (combined_file == NULL) {
        perror("無法打開執行檔");
        return EXIT_FAILURE;
    }

    // 查找壓縮檔的位置
    long zip_start = find_zip_start(combined_file);
    if (zip_start == -1) {
        fprintf(stderr, "無法找到壓縮檔\n");
        fclose(combined_file);
        return EXIT_FAILURE;
    }

    // 提取壓縮檔
    extract_zip(combined_file, zip_start);

    fclose(combined_file);
    printf("壓縮檔已提取為 extracted_config.zip\n");
    return 0;
}
