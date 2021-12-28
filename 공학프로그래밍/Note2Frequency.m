function frequency = Note2Frequency(LineLocation, NoteLocation)
%% 줄 찾아낼 구역 선정 (마지막 5개 줄 위로는 잘라냄)
%마지막 5개 선의 위치 추출
Last5Line = LineLocation(end-4:end); 
%음표의 위치를 마지막 줄로 선정하기 위한 기준
LineCreteria = (LineLocation(end-4) - LineLocation(end-5))/2+LineLocation(end-5); 
%기준 내 존재하는 음표 추출
InnerCreteria = find(NoteLocation(:,2)>LineCreteria);
NoteInLastLine5 = NoteLocation(InnerCreteria,:); %기준 내에 존재하는 음표의 위치 좌표 행렬
%% 음표 주파수 찾는 과정
% 한 음 당 간격 찾기
Distance2 = abs(sum(Last5Line-Last5Line(5))/10);
Distance = Distance2/2;

% frequecy가 총 12개라고 가정 (가운도부터 온음들만 비교) 
RepNoteY = repmat(NoteInLastLine5(:,2),1,12);
RepNoteX = repmat(NoteInLastLine5(:,1),1,12);
TempDistance = Distance*repmat([-2:9],length(NoteInLastLine5(:,2)),1);
% whereIs = RepNoteY+TempDistance-Last5Line(end);
%선과 비교해서 각 음의 존재여부 파악(열번호는 몇번째 음인지 나타냄)
WhichFrequencyIsON = abs((RepNoteY+TempDistance-Last5Line(end)))<(Distance/2);
frequency = (RepNoteX.*WhichFrequencyIsON)'>0;


