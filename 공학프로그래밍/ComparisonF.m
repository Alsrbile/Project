function [result1,result2] = ComparisonF(SoundFromScore,SoundFromRecording,ComparisonNum)
%% �� �� ��� : �� ���� record�࿡ �ϳ��� sound�� ��ġ���θ� ��
% �켱 record�� ������ ��� ���� �� �ְ� sound�� record�� ����
if sum(SoundFromRecording(:,end))~=0
TempScore = repmat(SoundFromScore(:,ComparisonNum(1)+1),1,3);%�Ǻ����� ���°�� ���ϴ��� Ȯ��
TempSound = SoundFromRecording(:,end-2:end);
IsCorrect = (sum(TempScore == TempSound)>=11); % 11�� �̻� ��ġ�ϴ��� Ȯ��

result1=sum(IsCorrect); % ��ġ�ϴ� ���� ��ȯ
result2 =(ComparisonNum(2)+1); % ���� ���ֵ� ������ �ľǵǸ� ī��Ʈ
else
result1 =0 ;
result2 = 0;
end

    
