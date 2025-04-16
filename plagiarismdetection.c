#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_DOCS 10
#define MAX_WORDS 1000
#define MAX_WORD_LEN 30
#define HASH_SIZE 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int freq[MAX_DOCS];
} WordEntry;

WordEntry hashTable[HASH_SIZE];
int docCount = 0;

int hash(char *word) {
    int h = 0;
    while (*word) {
        h = (h * 31 + tolower(*word)) % HASH_SIZE;
        word++;
    }
    return h;
}

void processDocument(char *text, int docId) {
    char *token = strtok(text, " .,;:\"\n\t\r");
    while (token) {
        int h = hash(token);
        while (hashTable[h].word[0] && strcmp(hashTable[h].word, token) != 0)
            h = (h + 1) % HASH_SIZE;

        if (hashTable[h].word[0] == '\0') {
            strcpy(hashTable[h].word, token);
        }
        hashTable[h].freq[docId]++;

        token = strtok(NULL, " .,;:\"\n\t\r");
    }
}

float cosineSimilarity(int doc1, int doc2) {
    float dot = 0, norm1 = 0, norm2 = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        int f1 = hashTable[i].freq[doc1];
        int f2 = hashTable[i].freq[doc2];
        dot += f1 * f2;
        norm1 += f1 * f1;
        norm2 += f2 * f2;
    }
    if (norm1 == 0 || norm2 == 0) return 0.0;
    return dot / (sqrt(norm1) * sqrt(norm2));
}

int main() {
    char *documents[MAX_DOCS] = {
        "This is a sample document used for plagiarism detection.",
        "Plagiarism detection is a very important task.",
        "This document is a sample used to detect plagiarism.",
        "Unrelated text that should not match much."
    };

    docCount = 4;

    for (int i = 0; i < docCount; i++) {
        char buffer[1000];
        strcpy(buffer, documents[i]);
        processDocument(buffer, i);
    }

    for (int i = 0; i < docCount; i++) {
        for (int j = i + 1; j < docCount; j++) {
            float sim = cosineSimilarity(i, j);
            printf("Similarity between Doc %d and Doc %d: %.2f%%\n", i, j, sim * 100);
        }
    }

    return 0;
}
