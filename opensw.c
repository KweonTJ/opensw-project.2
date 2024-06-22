#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tesseract/capi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_TEXT_LENGTH 10000

// 연결 리스트의 노드 구조체 정의
typedef struct Node {
    char filename[MAX_FILENAME_LENGTH];
    struct Node *next;
} Node;

// 연결 리스트의 헤드 노드 전역 변수
Node *head = NULL;

// 연결 리스트에 새로운 노드 추가
void addNode(const char *filename) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->filename, filename, MAX_FILENAME_LENGTH - 1);
    newNode->filename[MAX_FILENAME_LENGTH - 1] = '\0';
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        Node *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// 연결 리스트의 모든 노드 출력
void printList() {
    Node *current = head;
    printf("OCR Results:\n");
    while (current != NULL) {
        printf("- %s\n", current->filename);
        current = current->next;
    }
}

void performOCR(const char *inputImage, const char *outputFilename)
{
    // OpenCV를 사용하여 이미지 읽기
    IplImage *image = cvLoadImage(inputImage, CV_LOAD_IMAGE_GRAYSCALE);
    if (!image)
    {
        printf("Could not open or find the image\n");
        return;
    }

    // Tesseract API 초기화
    TessBaseAPI *ocr = TessBaseAPICreate();
    if (TessBaseAPIInit3(ocr, NULL, "eng+kor") != 0)
    {
        printf("Could not initialize tesseract.\n");
        TessBaseAPIEnd(ocr);
        TessBaseAPIFree(ocr);
        return;
    }

    TessBaseAPISetPageSegMode(ocr, PSM_AUTO);

    // Tesseract로 이미지 OCR 수행
    TessBaseAPISetImage2(ocr, image);
    char *outText = TessBaseAPIGetUTF8Text(ocr);

    // OCR 결과를 파일로 저장
    FILE *outFile = fopen(outputFilename, "w");
    if (outFile)
    {
        fputs(outText, outFile);
        fclose(outFile);
        printf("OCR result saved to %s\n", outputFilename);

        // 연결 리스트에 추가
        addNode(outputFilename);
    }
    else
    {
        printf("Could not open the file for writing.\n");
    }

    // 메모리 해제
    TessDeleteText(outText);
    TessBaseAPIEnd(ocr);
    TessBaseAPIFree(ocr);
    cvReleaseImage(&image);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_image>\n", argv[0]);
        return -1;
    }

    char inputImage[MAX_FILENAME_LENGTH];
    strncpy(inputImage, argv[1], MAX_FILENAME_LENGTH - 1);
    inputImage[MAX_FILENAME_LENGTH - 1] = '\0';

    // OCR 수행 및 결과 저장
    char outputFilename[MAX_FILENAME_LENGTH];
    printf("Enter output file name: ");
    fgets(outputFilename, MAX_FILENAME_LENGTH, stdin);
    outputFilename[strcspn(outputFilename, "\n")] = '\0';
    performOCR(inputImage, outputFilename);

    // 저장된 OCR 결과 불러오기
    char loadOption;
    printf("Do you want to load the saved OCR result? (y/n): ");
    scanf(" %c", &loadOption);
    if (loadOption == 'y' || loadOption == 'Y')
    {
        printList();
    } 

    // 연결 리스트 메모리 해제
    Node *current = head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;

    return 0;
}
