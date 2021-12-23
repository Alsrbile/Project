# 영상처리시스템 텀프로젝트 작품

### *"스도쿠 퍼즐 풀이 시스템 제작"*

* 영상 : <https://www.youtube.com/watch?v=6yQHSXSXcJA>

#### 제작 방식
* 데이터 제작

<img src="https://user-images.githubusercontent.com/70309556/147243214-8949347f-5cdf-4ee0-8f21-c2adc2236cfc.png" width="360px" height="100px" title="px(픽셀) 크기 설정" alt="RubberDuck"></img><br/>
-임의의 글꼴 12개 사용

* 문제풀이

<img src="https://user-images.githubusercontent.com/70309556/147243624-7a9c3d1f-9f45-4b4b-b9ed-3c248b19b095.png" width="360px" height="300px" title="px(픽셀) 크기 설정" alt="RubberDuck"></img><br/>
-KLT Corner Detection으로 4개의 점 검출
-4개의 점을 Perspective 변해서 왜곡없게 만들기
-KNN 학습을 통해 유추한 값을 list로 제작. (빈 공간은 0으로 채움)
-list로 얻은 값을 풀어낸다. (재귀함수 이용)

* 출력

<img src="https://user-images.githubusercontent.com/70309556/147244236-d5a4913c-dc7d-44ea-820e-80159eea6be0.png" width="290px" height="260px" title="px(픽셀) 크기 설정" alt="RubberDuck"></img><br/>
-cv2.bitwise, cv2.add 연산으로 숫자를 이미지 위에 출력
