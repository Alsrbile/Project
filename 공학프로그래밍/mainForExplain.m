% �̹���ó�� ���������Դϴ�.
% 1. �̹����� �޾Ƽ� y�� ���μ� ���� �ľ�
% 2. ��ǥ�� ��ǥ �޾Ƽ� ���� �ľ�
% 3. 1�� 2���� ���� �ڷḦ �������� sound vector ����
% 4. �տ� �����Ǹ� �ȵǴ� �κ� 2/3�� ���� ���� pass
clear;
clc;
LineLocation=[];
NoteLocation=[];
LineLocation = LineFinding('littlestar.jpg'); %���� ��ġ�� [a]�� ���
NoteLocation = noteselect('littlestar.jpg');
SoundVectorOfScore = Note2Frequency(LineLocation, NoteLocation); %�ش� soundvector ����

% ��ǥ�� ��ǥ�� [x y]�� ���
% ��µ� ��ǥ�� ���� ��ġ�� ��, soundMatrix ����
 

%% ���� �м�
%fft�� �ҿ�Ǵ� �ð� ���� �ʿ�
% ������ ������ �ҿ�Ǵ� �ð� ���� �ʿ�
% ���� 
%recoder��ü ����
a = 1;% �������� �Ѿ�� ������ record�� �� ����
count = zeros(1,2); %(1)���� ��� ��ġ�ߴ��� ���, 2���� ���� ���� ���� Ƚ�� ���
% ���� phrase�� ��Ÿ���� �� ��� (�������ٱ��� ���)
initialRecordingTime = 1;
RecordingTime = initialRecordingTime;
SoundVectorOfRecorder = [];
waitingCreteria =(length(NoteLocation)-length(SoundVectorOfScore))*2/3;
countMatch = 0;
while a == 1
% SoundData = zeros(12,1); % �̸� ������� Ȯ��
tic %������ ó���ð� Ȯ��
SoundData = SoundRecord(RecordingTime);
SoundVectorOfRecorder = [SoundVectorOfRecorder Sound2Frequency(SoundData)];

    if length(SoundVectorOfRecorder(1,:))<3
         continue;
    else 
%% �� �� ����
% �����ð� ���ֵ� ��, ���� ���ϵ��� ����
        [countMatch,count(2)]= ComparisonF(SoundVectorOfScore,SoundVectorOfRecorder,count);
        if (count(2) >waitingCreteria ) && (countMatch>=1)
        count(1) = count(1)+1; %���������� ���� ��ġ�ϴ� ���� check
        fprintf('count(��ġȽ��, �� ���� Ƚ��) = (%d,%d)\n',count(1),count(2))
            if count(1) >= length(SoundVectorOfScore(1,:))*0.8
            a= a+1;
            end
        else
            count(1) = 0; % Ʋ���� �ٽ� 0���� count
        end
    end

time = toc;
fprintf("�����ð� %f\n",time);
timeover = time - initialRecordingTime;
if timeover<10^-3
    continue;
else
RecordingTime = RecordingTime - 0.7*timeover; %����ġ�� ���� �༭ ���� �������� ���Ž�Ų��.
end
end
disp('nextpage')  %imshow('nextpage'); ������������ �̵� 