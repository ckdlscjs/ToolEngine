# DirectX MapTool
![dxmaptool](https://github.com/ckdlscjs/ToolEngine/assets/41976800/c3ec8ead-7709-4e6d-b816-2f125729f9e4)
![Diagram](https://github.com/ckdlscjs/ToolEngine/assets/41976800/16cb3c01-d431-40b3-b058-67a9cc3f43b5)
## 개요
-Engine 구조 설계

DX의 렌더링 파이프라인을 사용할 때 객체의 관리를 유동성 있게 처리하기 위해 ID3D11 인터페이스 객체들을 가지는
별도의 클래스들을 작업해 원하는 생성자 형태에 맞게 생성시키고 이를 소멸자에서 Release 시키는 RAII 구조를 사용해
메모리누수를 방지하기위한 프로젝트 디자인패턴을 설계함

SingletonManager들을 이용해 해당 객체들을 생성/삭제 하고 Frame,Render 함수 호출을 매니저에서 위임 시킴 으로써 프로그램 흐름에 일관성을 유지하는 방식으로 설계하였음

해당 엔진을 사용해 MapTool을 제작하였고 Tool을이용해
.Map이라는 별도의 확장자로 맵 데이터를 만들고
저장하여 활용
## 기술 스택
- C++
- DirectX
- FBX SDK
- FMOD

## 동영상
https://youtu.be/_ik09B4ptmo?si=l5gQW1pp_IqxO2Hu (MapTools)
