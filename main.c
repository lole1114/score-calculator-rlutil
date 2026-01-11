/*
 * Score Calculator (Text UI) using rlutil.h
 * Features:
 * - Menu UI (Up/Down + Enter, Esc to exit)
 * - Add scores (0~100), list scores
 * - Statistics: count, avg, max, min, pass rate (>=60)
 * - Clear all data
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rlutil.h"

#ifndef KEY_UP
#define KEY_UP 14
#endif
#ifndef KEY_DOWN
#define KEY_DOWN 15
#endif
#ifndef KEY_ENTER
#define KEY_ENTER 13
#endif
#ifndef KEY_ESCAPE
#define KEY_ESCAPE 27
#endif

#define MAX_SCORES 200

static int scores[MAX_SCORES];
static int scoreCount = 0;

static void drawHeader(const char* subtitle) {
    setColor(LIGHTCYAN);
    locate(3, 2);
    printf("=== Score Calculator (rlutil.h Text UI) ===");
    setColor(WHITE);

    locate(3, 3);
    printf("%s", subtitle ? subtitle : "Use Up/Down to move, Enter to select, Esc to exit.");

    setColor(LIGHTBLACK);
    locate(3, 4);
    printf("Data: %d/%d scores", scoreCount, MAX_SCORES);
    setColor(WHITE);
}

static void clearLine(int x, int y, int width) {
    locate(x, y);
    for (int i = 0; i < width; i++) putchar(' ');
}

static void showMessage(const char* msg, int color) {
    clearLine(3, 20, 70);
    locate(3, 20);
    setColor(color);
    printf("%s", msg);
    setColor(WHITE);
}

static void pauseToBack(void) {
    setColor(LIGHTYELLOW);
    locate(3, 22);
    printf("Press any key to go back...");
    setColor(WHITE);
    getkey();
}

static int readIntAt(int x, int y) {
    // 讓輸入位置固定，畫面比較像 UI
    char buf[64];
    clearLine(x, y, 30);
    locate(x, y);
    setColor(WHITE);
    fflush(stdout);

    if (scanf("%63s", buf) != 1) return 0;

    // 基礎字串轉整數（避免輸入亂碼直接炸）
    char* endptr = NULL;
    long v = strtol(buf, &endptr, 10);
    if (endptr == buf || *endptr != '\0') return 0;
    return (int)v;
}

static void screenAddScore(void) {
    cls();
    drawHeader("Add scores (0~100). Input -1 to finish.");

    int y = 7;
    locate(3, y);     printf("Enter score: ");
    locate(3, y + 2); printf("Tips: You can input multiple scores continuously.");

    while (1) {
        if (scoreCount >= MAX_SCORES) {
            showMessage("Storage is full. Cannot add more.", LIGHTRED);
            pauseToBack();
            return;
        }

        locate(16, y);
        int v = readIntAt(16, y);

        if (v == -1) {
            showMessage("Finish adding scores.", LIGHTGREEN);
            pauseToBack();
            return;
        }

        if (v < 0 || v > 100) {
            showMessage("Invalid input! Please enter 0~100 or -1 to finish.", LIGHTRED);
            continue;
        }

        scores[scoreCount++] = v;
        showMessage("Score added.", LIGHTGREEN);

        // 下一次輸入時把游標放回去
        locate(16, y);
    }
}

static void screenListScores(void) {
    cls();
    drawHeader("List all scores.");

    if (scoreCount == 0) {
        locate(3, 7);
        setColor(LIGHTYELLOW);
        printf("No scores yet. Please add scores first.");
        setColor(WHITE);
        pauseToBack();
        return;
    }

    locate(3, 7);
    setColor(LIGHTGREEN);
    printf("Index   Score");
    setColor(WHITE);

    int startY = 9;
    for (int i = 0; i < scoreCount; i++) {
        locate(3, startY + i);
        printf("%5d   %5d", i + 1, scores[i]);

        // 超出螢幕就停止，避免畫面亂掉（也比較像成品）
        if (startY + i >= 19) {
            locate(3, 20);
            setColor(LIGHTYELLOW);
            printf("(Only first %d shown on screen)", i + 1);
            setColor(WHITE);
            break;
        }
    }

    pauseToBack();
}

static void screenStats(void) {
    cls();
    drawHeader("Statistics.");

    if (scoreCount == 0) {
        locate(3, 7);
        setColor(LIGHTYELLOW);
        printf("No scores yet. Please add scores first.");
        setColor(WHITE);
        pauseToBack();
        return;
    }

    int sum = 0, max = scores[0], min = scores[0], pass = 0;
    for (int i = 0; i < scoreCount; i++) {
        int v = scores[i];
        sum += v;
        if (v > max) max = v;
        if (v < min) min = v;
        if (v >= 60) pass++;
    }

    double avg = (double)sum / scoreCount;
    double passRate = 100.0 * pass / scoreCount;

    locate(3, 7);
    setColor(LIGHTGREEN);
    printf("Result Summary");
    setColor(WHITE);

    locate(3, 9);  printf("Count     : %d", scoreCount);
    locate(3, 10); printf("Average   : %.2f", avg);
    locate(3, 11); printf("Maximum   : %d", max);
    locate(3, 12); printf("Minimum   : %d", min);
    locate(3, 13); printf("Pass (>=60): %d", pass);
    locate(3, 14); printf("Pass Rate : %.1f%%", passRate);

    pauseToBack();
}

static void screenClearAll(void) {
    cls();
    drawHeader("Clear all data.");

    locate(3, 7);
    setColor(LIGHTRED);
    printf("This will delete ALL scores.");
    setColor(WHITE);

    locate(3, 9);
    printf("Press Enter to confirm, Esc to cancel.");

    while (1) {
        int k = getkey();
        if (k == KEY_ENTER) {
            scoreCount = 0;
            showMessage("All scores cleared.", LIGHTGREEN);
            pauseToBack();
            return;
        }
        if (k == KEY_ESCAPE) {
            showMessage("Canceled.", LIGHTYELLOW);
            pauseToBack();
            return;
        }
    }
}

static void screenAbout(void) {
    cls();
    drawHeader("About this program.");

    locate(3, 7);
    printf("This is a Text UI Score Calculator written in C.");

    locate(3, 9);
    printf("Library: rlutil.h (color, cursor, key input)");

    locate(3, 11);
    printf("Functions:");
    printf("\n  - Add score (0~100)");
    printf("\n  - List scores");
    printf("\n  - Statistics (avg/max/min/pass rate)");
    printf("\n  - Clear all");

    pauseToBack();
}

static void drawMenu(int selected) {
    const char* items[] = {
        "Add Score",
        "List Scores",
        "Statistics",
        "Clear All",
        "About",
        "Exit"
    };
    const int n = (int)(sizeof(items) / sizeof(items[0]));

    int startY = 7;
    for (int i = 0; i < n; i++) {
        locate(5, startY + i);

        if (i == selected) {
            setColor(BLACK);
            setBackgroundColor(LIGHTGREEN);
            printf(" > %-20s ", items[i]);
            setBackgroundColor(BLACK);
            setColor(WHITE);
        } else {
            setColor(WHITE);
            printf("   %-20s", items[i]);
        }
    }

    setColor(LIGHTBLACK);
    locate(3, 16);
    printf("Controls: Up/Down = move | Enter = select | Esc = exit");
    setColor(WHITE);
}

int main(void) {
    int selected = 0;

    while (1) {
        cls();
        drawHeader(NULL);
        drawMenu(selected);

        int k = getkey();

        if (k == KEY_ESCAPE) break;

        if (k == KEY_UP) {
            selected = (selected - 1 + 6) % 6;
        } else if (k == KEY_DOWN) {
            selected = (selected + 1) % 6;
        } else if (k == KEY_ENTER) {
            switch (selected) {
                case 0: screenAddScore(); break;
                case 1: screenListScores(); break;
                case 2: screenStats(); break;
                case 3: screenClearAll(); break;
                case 4: screenAbout(); break;
                case 5: cls(); return 0;
            }
        }
    }

    cls();
    return 0;
}
