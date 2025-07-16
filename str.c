#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h> // 処理時間計測のためにインクルード済み

#define TEXT_MAX    (1024 * 1024 * 10)  /* 10MB */
#define PATTERN_MAX (1024)

char    T[TEXT_MAX];
char    P[PATTERN_MAX];
int     counter_compare;

void Clear() {
    counter_compare = 0;
}

void Counter() {
    printf("Compare %d\n", counter_compare);
}

int my_compare(const char a, const char b) {
    counter_compare++;
    return (a == b) ? 1 : 0;
}

int BruteForce(char *T, int n, char *P, int m) {
    int i, p;
    /* For all characters in T[0] ... T[n-m] */
    for (i = 0; i <= n - m; i++) {
        /* For all characters in P[0] ... P[m-1] */
        for (p = 0; p < m; p++)
            /* If T[i+p] != P[p] */
            if (my_compare(T[i + p], P[p]) == 0)
                break;
        if (p == m) return i;
    }
    return -1;
}

void BruteForce_func(char *T, char *P) {
    int ptr;
    if ((ptr = BruteForce(T, strlen(T), P, strlen(P))) != -1)
        printf("Found at T[%d]\n", ptr);
    else
        printf("Not found \n");
}

int KMP(char *T, int n, char *P, int m) {
    int i, p;
    int table[PATTERN_MAX];
    /* Make a skip table */
    for (p = -1, i = 0; i < m; i++) {
        table[i] = p;
        while ((p >= 0) && (P[i] != P[p]))
            p = table[p];
        p++;
    }
    /* Print the skip table */
    //for (i = 0; i < m; i++)
    //    printf("%d ", table[i]);
    //printf("\n");
    /* String match */
    for (p = 0, i = 0; i < n; i++) {
        while ((p >= 0) && (my_compare(T[i], P[p]) == 0))
            p = table[p];
        if (p == m - 1)
            return i - m + 1;
        p++;
    }
    return -1;
}

void KMP_func(char *T, char *P) {
    int ptr;
    if ((ptr = KMP(T, strlen(T), P, strlen(P))) != -1)
        printf("Found at T[%d]\n", ptr);
    else
        printf("Not found \n");
}

int BM(char *T, int n, char *P, int m) {
    int i, p;
    int table[UCHAR_MAX + 1];
    /* Make a skip table */
    for (i = 0; i <= UCHAR_MAX; i++){
        table[i] = m;
    }
    for (i = 0; i < m; i++){
        table[(int)P[i]] = m - i - 1;
    }

    /* Print the skip table */
    for (i = '0'; i < 'z'; i++)
        if (table[i] != m)
            printf("[%c]=%d ", i, table[i]);
    printf("\n");
    /* String match */
    i = m - 1;
    while (i < n) {
        p = m - 1;
        while (my_compare(T[i], P[p])) {
            if (p == 0) return i;
            p--; i--;
        }
        i += (table[ (int)T[i] ] > m - p) ?
            table[ (int) T[i] ] : m - p;
    }
    return -1;
}

void BM_func(char *T, char *P) {
    int ptr;
    if ((ptr = BM(T, strlen(T), P, strlen(P))) != -1)
        printf("Found at T[%d]\n", ptr);
    else
        printf("Not found \n");
}

#define D 256
#define Q 99991
int verify_match(const char *T, const char *P, int m) {
    for (int i = 0; i < m; i++) {
        if (my_compare(T[i], P[i]) == 0) {
            return 0;
        }
    }
    return 1;
}
int RabinKarp(char *T, int n, char *P, int m) {
    // --- 修正: intをlong longに変更してオーバーフローを防ぐ ---
    long long p_hash = 0;
    long long t_hash = 0;
    long long h = 1;
    int i;

    if (m > n) return -1;
    for (i = 0; i < m - 1; i++) {
        h = (h * D) % Q;
    }
    for (i = 0; i < m; i++) {
        p_hash = (D * p_hash + P[i]) % Q;
        t_hash = (D * t_hash + T[i]) % Q;
    }

    //printf("Pattern Hash: %lld\n", p_hash);
    
    for (i = 0; i <= n - m; i++) {
        counter_compare++;
        if (p_hash == t_hash) {
            if (verify_match(&T[i], P, m)) {
                return i;
            }
        }
        if (i < n - m) {
            t_hash = (D * (t_hash - T[i] * h) + T[i + m]) % Q;
            if (t_hash < 0) {
                t_hash = t_hash + Q;
            }
        }
    }
    return -1;
}
void RabinKarp_func(char *T, char *P) {
    int ptr;
    if ((ptr = RabinKarp(T, strlen(T), P, strlen(P))) != -1)
        printf("Found at T[%d]\n", ptr);
    else
        printf("Not found \n");
}

// --- 追加: ビット列を2進数で表示するためのヘルパー関数 ---
void print_binary(unsigned long long n, int bits) {
    int i;
    for (i = bits - 1; i >= 0; i--) {
        putchar((n >> i) & 1 ? '1' : '0');
    }
}

int ShiftAnd(char *T, int n, char *P, int m) {
    int i;
    unsigned long long mask[UCHAR_MAX + 1];
    unsigned long long state = 0;

    if (m > 64) {
        printf("Error: Pattern length must be <= 64 for this Shift-And implementation.\n");
        return -1;
    }

    for (i = 0; i <= UCHAR_MAX; i++) {
        mask[i] = ~0ULL;
    }
    
    for (i = 0; i < m; i++) {
        mask[(unsigned char)P[i]] &= ~(1ULL << i);
    }

    //printf("Shift-And Masks (for pattern chars, length %d):\n", m);
    for (i = 0; i < m; i++) {
        int first_occurrence = 1;
        for (int j = 0; j < i; j++) {
            if (P[j] == P[i]) {
                first_occurrence = 0;
                break;
            }
        }

    }

    for (i = 0; i < n; i++) {
        counter_compare++;
        state = (state << 1) | mask[(unsigned char)T[i]];
        
        if ((state & (1ULL << (m - 1))) == 0) {
            return i - m + 1;
        }
    }
    
    return -1;
}

void ShiftAnd_func(char *T, char *P) {
    int ptr;
    if ((ptr = ShiftAnd(T, strlen(T), P, strlen(P))) != -1)
        printf("Found at T[%d]\n", ptr);
    else
        printf("Not found \n");
}


int main(int argc, char *argv[]) {
    FILE *fp;
    char command;
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]); exit(1);
    }
    if ((fp = fopen(argv[1], "r")) == NULL) {
        printf("Cannot open file (%s) \n", argv[1]); exit(1);
    }
    /* Read the file to get the pattern P */
    fgets(P, PATTERN_MAX, fp);
    P[strlen(P) - 1] = 0;
    /* Read the file to get the text T */
    fgets(T, TEXT_MAX, fp);
    T[strlen(T) - 1] = 0;
    fclose(fp);
    //printf("P[%d] = %s \n", strlen(P), P);
    //printf("T[%d] = %s \n", strlen(T), T);

    Clear();
    printf("[b]       Brute-Force Algorithm\n");
    printf("[k]       Knuth-Morris-Pratt Algorithm\n");
    printf("[m]       Boyer-Moore Algorithm\n");
    printf("[r]       Rabin-Karp Algorithm\n");
    printf("[s]       Shift-And Algorithm\n");
    printf("[e]       Exit\n");
    for (;;) {
        printf("\nEnter command: ");
        scanf(" %c", &command);

        // --- ここからが変更部分 ---
        // 10回の実行結果を格納するための変数
        long total_time = 0;
        long total_compares = 0;
        int i;
        const int RUN_COUNT = 10; // 実行回数

        // 'e'(終了)と無効なコマンドはループ前に処理
        if (command == 'e') return 0;
        if (strchr("bkmrs", command) == NULL) {
            printf("Invalid command.\n");
            continue;
        }

        // 選択されたアルゴリズムを10回実行して平均を計測
        for (i = 0; i < RUN_COUNT; i++) {
            Clear(); // 毎回の実行前に比較カウンターをリセット
            struct timeval start, end;

            // 1回目の実行時のみ、テーブルや結果などの詳細情報を表示する
            if (i == 0) {
                //printf("--- Running algorithm (1st run with details) ---\n");
                gettimeofday(&start, NULL);
                switch (command) {
                    case 'b': BruteForce_func(T, P); break;
                    case 'k': KMP_func(T, P);        break;
                    case 'm': BM_func(T, P);         break;
                    case 'r': RabinKarp_func(T, P);  break;
                    case 's': ShiftAnd_func(T, P);   break;
                }
                gettimeofday(&end, NULL);
            } else {
            // 2回目以降は純粋な時間計測のため、結果表示をしないコア関数を呼ぶ
                gettimeofday(&start, NULL);
                switch (command) {
                    case 'b': BruteForce(T, strlen(T), P, strlen(P)); break;
                    case 'k': KMP(T, strlen(T), P, strlen(P));        break;
                    case 'm': BM(T, strlen(T), P, strlen(P));         break;
                    case 'r': RabinKarp(T, strlen(T), P, strlen(P));  break;
                    case 's': ShiftAnd(T, strlen(T), P, strlen(P));   break;
                }
                gettimeofday(&end, NULL);
            }
            
            // 合計時間に加算
            total_time += (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
            // 合計比較回数に加算
            total_compares += counter_compare;
        }
        
        // --- 平均結果の表示 ---
        printf("----------------------------------------\n");
        printf("Average over %d runs:\n", RUN_COUNT);
        // 平均比較回数を表示
        printf("Compares: %ld\n", total_compares / RUN_COUNT);
        // 平均実行時間をマイクロ秒で表示
        printf("  Average Execution time: %ld us\n", total_time / RUN_COUNT);
        printf("----------------------------------------\n");
        
        Clear(); // 次のコマンド入力に備えてカウンターをリセット
        // --- ここまでが変更部分 ---
    }
    return 0;
}
