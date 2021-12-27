function [y] = LineFinding(filename)

%morphology에 대한 궁금증은 문서 bwmorph 참조할 것
Image1 = imread(filename); %이미지 불러오기
ImageGray = rgb2gray(Image1); %RGB를 회색조로 변환
ImageBW = imbinarize(ImageGray,0.6); %회색조 이미지를 이진화
ImageRBW = (ImageBW==0);%흑백 반전
mask1 = ones(1,20);% 가로로 긴 마스크 생성
ImageLine = imerode(ImageRBW, mask1);%침식을 이용한 백색 가로선 제외 제거
ImageThinLine = bwmorph(ImageLine,'thin'); % 이미지를 세선화
% 먼저 선 추출 후 세선화했더니, 값이 출력이 안됨..
temp1 = sum(ImageThinLine'); %sum은 열방향으로 합을 구함/ 따라서 transpose해서 행의 합을 구함
% y축으로 히스토그램 작성
[a b] = size(Image1);
temp2 = temp1>0.5*a;%이미지의 행 길이의 50%보다 짧은 선 제거
y = find(temp2);%가로선이 존재하는 y축의 좌표 추출
end