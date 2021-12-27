function result = noteselect(filename)

%흑백화, 이진화, 반전
img = imread(filename); %이미지 불러오기
imgGRAY = rgb2gray(img);% RGB에서 흑백조
imgREV= imcomplement(imgGRAY); %흑백 반전
imgFNL=imbinarize(imgREV); %이진화

%모서리 무늬 지우기
[imgX, imgY] = size(imgFNL);
imgFNL(1:3,:) = 0;
imgFNL(imgX-3:imgX,:) = 0;
imgFNL(:,1:3) = 0;
imgFNL(:,imgX-3:imgX) = 0; %모서리 3번째까지 0으로 만든다.

se1 = strel('line', 50, 0);      %가로로긴 선을 이용해 오선 검출
IMG= imopen(imgFNL,se1);
IMG_thin = bwmorph(IMG, 'thin'); 

IMG1 = bwmorph(imgFNL,'thin');
IMG_Re = imreconstruct(IMG_thin, IMG1);
IMG_sub = IMG_Re - IMG_thin; %오선 빼내기(기존악보에서 오선만 빼기)

IMG_BR = bwmorph(IMG_sub, 'bridge'); %오선빼고 끊긴 사이 1개 간격 매꾸기

se2 = strel('disk',1);               % 머리가 octagon크기 3보다 작으면 검출이 안되므로 팽창시킨다. 
imgimg = imdilate(IMG_BR,se2);

imgimgimg = bwmorph(imgimg, 'thin'); % 선을 얇게함

IMG_holes = imfill(imgimgimg, 4, 'holes'); %구멍채우기


%% 높은 음자리표 길이차이로 구하기

%오선 개수 검출
THIN_number = bwlabel(IMG_thin);
MAX_thin = sum(THIN_number');
MAX_thin_one = nnz(MAX_thin);
MAX_thin_number = 5*round(MAX_thin_one/5); %튀는 값이 존재하므로 평균으로 구함

%오선 사이 간격 구하기
Pos_line1 = find(sum((THIN_number==1)'));
Pos_line5 = find(sum((THIN_number==5)'));
Interval_line = Pos_line5-Pos_line1; 

%구멍만 나오게하기
hoehoe = imdilate(IMG_holes, se2);
se3=strel('octagon' ,3);
IMG_Head22 = imopen(hoehoe, se3);
IMG_2Hole = imreconstruct(IMG_Head22,IMG_holes);
IMG_3Hole = imerode(IMG_2Hole,se4);  %세로줄만 검출

% 음표만 남기기 위한 for문에 필요한 값들
A = bwlabel(IMG_holes);
MAX = max(max(A));
A_select_zero = (zeros([imgX,imgY])); %모든 값이 0에서 시작해서 라벨값이 작으면 합친다.

cnt=1; % Counter

for i=1:1:MAX %라벨 부여하고 하나씩 행합을 사용해 원하는값 검출. 
    
   Select_Label = (A==i); 
   HC_select = sum(Select_Label'); %열합을 행합으로 
   
   [LSx,LSy] = max(sum(Select_Label)); %열합으로 각 라벨의 큰 곳을 찾아서 +1위치부터의 꼬리지움
   
   Select_Label(:,(LSy+2):imgY) = 0;
   [XX,Line_select] = size(find(sum(Select_Label')==1));
   
   % 각라벨의 행합에서 1의 개수가 오선길이*0.45보다 작으면 지운다. 
   if(Line_select<((Interval_line)*0.45))         %꼬리가 오른쪽으로 모습이 잡히니 오른쪽을 지우고 행합을한다.
       Select_Label = (zeros([imgX,imgY]));        
   end                                         
   
   A_select_zero = A_select_zero + Select_Label;
end

A_select_zero = imreconstruct(imbinarize(A_select_zero),IMG_holes); %검출한 객체들 모양 복원


%% 구멍 존재하는 머리 빼내기

A_select_zero = imdilate(A_select_zero,se2); % 머리검출 쉽게하기위해 크기를 키움

se4=strel('octagon' ,3);
IMG_Head = imopen(A_select_zero, se4); %머리만 남김

se5 = strel('disk', 1);
B_select_zero1 = imdilate(IMG_BR, se5);           % 구멍있는 머리만 찾기위한 노가다작업
B_select_zero2 = bwmorph(B_select_zero1, 'skel');
B_select_zero3 = bwmorph(B_select_zero2, 'fill');
B_select_zero4 = bwmorph(B_select_zero3, 'remove');
B_select_zero = imfill(B_select_zero4, 'holes');
IMG_HOLE = imopen(B_select_zero, se4);

IMG_LEFT = imbinarize(B_select_zero-B_select_zero1);  % 구멍있는 객체로 빈공간 채운 객체를 빼서 남는 곳 검출 

NOTE_HOLE = imreconstruct(IMG_LEFT,IMG_Head); %빈 음표만 가져온다.

P1 = IMG_Head;
P2 = imreconstruct(NOTE_HOLE,P1);

se6=strel('disk',3);

IMG_Head2 = imclose(P1, se6); %너무 각지지않고 둥글하게 만듬
IMG_Head3 = imclose(P2, se6);
%% 무게중심

[L, n] = bwlabel(IMG_Head2); %무게중심 좌표 저장과정
result = zeros(n,2);
   for k = 1:n
    [r,c] = find(L == k);
    result(k,1) = mean(c);
    result(k,2) = mean(r);
   end
   
[L2, n2] = bwlabel(IMG_Head3); %빈 노트 무게중심 좌표 저장과정
resultN = zeros(n2,2);
   for k = 1:n2
    [r,c] = find(L2 == k);
    resultN(k,1) = mean(c)+1;
    resultN(k,2) = mean(r);
   end
   result = [result; resultN];
   result = sortrows(result);% x축 순서대로 음표들을 정렬
%end