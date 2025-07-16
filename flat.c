#include <stdio.h>
#include <stdlib.h>

/**
 * @brief テキストファイルから改行と非ASCII文字を削除し、
 * 純粋なASCII文字のみの1行の文字列として標準出力するプログラム
 */
int main(int argc, char *argv[]) {
    FILE *inputFile;
    int ch;

    // --- 1. コマンドライン引数のチェック ---
    // プログラム実行時にファイル名が指定されているかを確認する
    if (argc != 2) {
        // 指定されていない場合、使い方を標準エラー出力に表示して終了
        fprintf(stderr, "使い方: %s <入力ファイル名>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // --- 2. 入力ファイルのオープン ---
    // 読み込みモード("r")でファイルを開く
    inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        // ファイルが開けなかった場合、エラーメッセージを表示して終了
        perror("入力ファイルを開けません");
        exit(EXIT_FAILURE);
    }

    // --- 3. 1文字ずつ読み込んで処理 ---
    // ファイルの終端(EOF)に達するまで、1文字ずつ読み込みを繰り返す
    while ((ch = fgetc(inputFile)) != EOF) {
        // --- ここからが変更部分 ---
        // 読み込んだ文字が標準的なASCII文字(0～127)の範囲内であり、
        // かつ、改行(\n)でも復帰(\r)でもない場合のみ出力する
        if (ch <= 127 && ch != '\n' && ch != '\r') {
            putchar(ch);
        }
        // --- ここまでが変更部分 ---
    }

    // --- 4. ファイルのクローズ ---
    fclose(inputFile);

    // ターミナルの表示を整えるため、最後に改行を一つだけ出力する
    printf("\n");

    return 0; // 正常終了
}
