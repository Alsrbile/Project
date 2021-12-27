function [result1,result2] = ComparisonF(SoundFromScore,SoundFromRecording,ComparisonNum)
%% 음 비교 방법 : 세 개의 record행에 하나의 sound와 일치여부를 비교
% 우선 record를 세개의 행과 비교할 수 있게 sound와 record를 복제
if sum(SoundFromRecording(:,end))~=0
TempScore = repmat(SoundFromScore(:,ComparisonNum(1)+1),1,3);%악보에서 몇번째를 비교하는지 확인
TempSound = SoundFromRecording(:,end-2:end);
IsCorrect = (sum(TempScore == TempSound)>=11); % 11개 이상 일치하는지 확인

result1=sum(IsCorrect); % 일치하는 갯수 반환
result2 =(ComparisonNum(2)+1); % 음이 연주된 것으로 파악되면 카운트
else
result1 =0 ;
result2 = 0;
end

    
