#include <iostream>
#include <string>
#include <fstream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;

// 연결 리스트 노드 정의
struct ListNode 
{
    string line;
    ListNode* next;
};

// 연결 리스트에 노드 추가
void appendNode(ListNode*& head, const string& line) 
{
    ListNode* newNode = new ListNode{ line, nullptr };
    if (!head) 
    {
        head = newNode;
    } else 
    {
        ListNode* current = head;
        while (current->next) 
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// 연결 리스트 출력 및 메모리 해제
void printAndFreeList(ListNode* head) 
{
    ListNode* current = head;
    while (current) 
    {
        cout << current->line << endl;
        ListNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

void performOCR(const string& inputImage, ListNode*& head) 
{
    // OpenCV를 사용하여 이미지 읽기
    Mat image = imread(inputImage, IMREAD_COLOR);
    if (image.empty()) 
    {
        cout << "Could not open or find the image" << endl;
        return;
    }

    // 그레이스케일로 변환
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // 이진화
    Mat binary;
    threshold(gray, binary, 150, 255, THRESH_BINARY);

    // Tesseract API 초기화
    tesseract::TessBaseAPI* ocr = new tesseract::TessBaseAPI();
    if (ocr->Init(NULL, "eng+kor", tesseract::OEM_LSTM_ONLY)) 
    {
        cout << "Could not initialize tesseract." << endl;
        delete ocr;
        return;
    }

    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    // OpenCV Mat을 Leptonica Pix로 변환
    Pix* pix = pixCreate(binary.cols, binary.rows, 8);
    for (int y = 0; y < binary.rows; y++)
    {
        for (int x = 0; x < binary.cols; x++) 
        {
            pixSetPixel(pix, x, y, binary.at<uchar>(y, x));
        }
    }

    // Tesseract로 이미지 OCR 수행
    ocr->SetImage(pix);
    char* outText = ocr->GetUTF8Text();

    // OCR 결과를 연결 리스트에 저장
    stringstream ss(outText);
    string line;
    while (getline(ss, line)) 
    {
        appendNode(head, line);
    }

    // 메모리 해제
    ocr->End();
    delete[] outText;
    pixDestroy(&pix);
    delete ocr;
}

void saveOCRResult(const string& filename, ListNode* head) 
{
    ofstream outFile(filename);
    if (outFile.is_open()) 
    {
        ListNode* current = head;
        while (current) 
        {
            outFile << current->line << endl;
            current = current->next;
        }
        outFile.close();
        cout << "OCR result saved to " << filename << endl;
    } else {
        cout << "Could not open the file for writing." << endl;
    }
}

void loadOCRResult(const string& filename) 
{
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        cout << "Content of " << filename << ":" << endl;
        while (getline(inFile, line)) 
        {
            cout << line << endl;
        }
        inFile.close();
    } 
    else 
    {
        cout << "Could not open the file for reading." << endl;
    }
}

int main(int argc, char** argv) 
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <input_image>" << endl;
        return -1;
    }

    string inputImage = argv[1];
    ListNode* head = nullptr;

    // OCR 수행 및 결과를 연결 리스트에 저장
    performOCR(inputImage, head);

    // 결과 파일에 저장
    string outputFilename;
    cout << "Enter output file name: ";
    cin.ignore(); // cin 버퍼 클리어
    getline(cin, outputFilename);
    saveOCRResult(outputFilename, head);

    // 저장된 OCR 결과 불러오기
    char loadOption;
    cout << "Do you want to load the saved OCR result? (y/n): ";
    cin >> loadOption;
    if (loadOption == 'y' || loadOption == 'Y') 
    {
        loadOCRResult(outputFilename);
    }

    // 연결 리스트 출력 및 메모리 해제
    printAndFreeList(head);

    return 0;
}
