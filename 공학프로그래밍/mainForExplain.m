% 이미지처리 메인파일입니다.
% 1. 이미지를 받아서 y축 가로선 분포 파악
% 2. 음표의 좌표 받아서 분포 파악
% 3. 1과 2에서 얻은 자료를 바탕으로 sound vector 생성
% 4. 앞에 감지되면 안되는 부분 2/3은 감지 없이 pass
clear;
clc;
LineLocation=[];
NoteLocation=[];
LineLocation = LineFinding('littlestar.jpg'); %선의 위치를 [a]로 출력
NoteLocation = noteselect('littlestar.jpg');
SoundVectorOfScore = Note2Frequency(LineLocation, NoteLocation); %해당 soundvector 추출

% 음표의 좌표를 [x y]로 출력
% 출력된 음표를 선의 위치와 비교, soundMatrix 생성
 

%% 음향 분석
%fft에 소요되는 시간 측정 필요
% 데이터 녹음에 소요되는 시간 측정 필요
% 녹음 
%recoder객체 생성
a = 1;% 페이지가 넘어가기 전까지 record와 비교 수행
count = zeros(1,2); %(1)에는 몇번 일치했는지 기록, 2에는 연속 녹음 진행 횟수 기록
% 같은 phrase가 나타나기 전 대기 (마지막줄까지 대기)
initialRecordingTime = 1;
RecordingTime = initialRecordingTime;
SoundVectorOfRecorder = [];
waitingCreteria =(length(NoteLocation)-length(SoundVectorOfScore))*2/3;
countMatch = 0;
while a == 1
% SoundData = zeros(12,1); % 미리 저장공간 확보
tic %데이터 처리시간 확인
SoundData = SoundRecord(RecordingTime);
SoundVectorOfRecorder = [SoundVectorOfRecorder Sound2Frequency(SoundData)];

    if length(SoundVectorOfRecorder(1,:))<3
         continue;
    else 
%% 논리 비교 과정
% 일정시간 연주된 후, 논리를 비교하도록 설정
        [countMatch,count(2)]= ComparisonF(SoundVectorOfScore,SoundVectorOfRecorder,count);
        if (count(2) >waitingCreteria ) && (countMatch>=1)
        count(1) = count(1)+1; %연속적으로 음이 일치하는 정도 check
        fprintf('count(일치횟수, 음 감지 횟수) = (%d,%d)\n',count(1),count(2))
            if count(1) >= length(SoundVectorOfScore(1,:))*0.8
            a= a+1;
            end
        else
            count(1) = 0; % 틀리면 다시 0부터 count
        end
    end

time = toc;
fprintf("녹음시간 %f\n",time);
timeover = time - initialRecordingTime;
if timeover<10^-3
    continue;
else
RecordingTime = RecordingTime - 0.7*timeover; %가중치를 적게 줘서 적은 진동으로 수렴시킨다.
end
end
disp('nextpage')  %imshow('nextpage'); 다음페이지로 이동 