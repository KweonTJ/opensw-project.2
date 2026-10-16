# OpenSW Project 2 — OCR Text Recognition

OpenCV와 Tesseract OCR을 활용하여 이미지 속 문자를 인식하고, 연결 리스트 자료구조를 이용해 인식 결과를 관리하는 C/C++ 기반 프로젝트입니다.

## 1. Project Overview

- **목적:** 이미지에서 한글 및 영문 텍스트 추출
- **개발 언어:** C, C++
- **주요 기술:** OpenCV, Tesseract OCR, Leptonica
- **자료구조:** Singly Linked List
- **실행 환경:** Linux 기반 콘솔 프로그램

## 2. Key Features

- **Image Processing**
  - OpenCV를 활용한 이미지 입력
  - Grayscale 변환 및 이진화 전처리(C++ 버전)
- **Optical Character Recognition**
  - Tesseract OCR 기반 한글·영문 문자 인식
  - 이미지 내 문자 데이터를 UTF-8 텍스트로 변환
- **Data Management**
  - 연결 리스트를 활용한 인식 결과 저장 및 관리
  - 텍스트 파일 저장 및 불러오기
  - 콘솔 환경에서 OCR 결과 출력

## 3. Source Code

- [`opensw.c`](opensw.c): Tesseract C API 기반 OCR 구현 및 출력 파일명 연결 리스트 관리
- [`OPENSW/opensw.cpp`](OPENSW/opensw.cpp): C++ 기반 이미지 전처리, OCR 결과의 행 단위 연결 리스트 저장 및 파일 입출력
- [`OPENSW/`](OPENSW/): C++ 소스코드 및 VS Code 빌드·디버깅 설정 파일

## 4. Development Notes

- OpenCV, Tesseract 및 Leptonica 라이브러리 필요
- 한글·영문 OCR을 위한 `kor`, `eng` 언어 데이터 필요
- 개발 당시의 소스코드로, 최신 라이브러리 환경에서는 일부 코드 수정이 필요할 수 있음
