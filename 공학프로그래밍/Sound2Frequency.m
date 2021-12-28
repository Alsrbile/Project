function result = Sound2Frequency(record)
Frequency = abs(fft(record)/length(record)); %주파수별 음압의 크기만 추출
% 켤레복소수로 인한 대칭부분 합으로 연산
Frequency2 = Frequency(1:fix(end/2)+1); 
Frequency2(2:end-1) = 2*Frequency2(2:end-1);
F = 44100*[0:length(Frequency)/2]/length(Frequency);
temp = F(find(Frequency2>max(Frequency2)*0.6));% 임계치 이상으로 큰 기본 주파수만 뽑아냄
% 얻은 주파수가 어느 음에 일치하는지 12개에 음에 대해 check
tempresult = zeros(length(temp),12) ;
    for k=1:length(temp)
    reptemp = repmat(temp(k),1,12);
    expcenterFrequency = [-9 -7 -5 -4 -2 0 2 3 5 7 9 11]/12;
    bandwidth = 440*(2.^(expcenterFrequency+1/24)-2.^(expcenterFrequency-1/24));
    CenterFrequency = 440*(2.^(expcenterFrequency));
    IsFrequency = abs(reptemp-CenterFrequency)<bandwidth;
    tempresult(k,:) = IsFrequency;
    end
    
    if k == 1
        result = tempresult';
    else
        result = (sum(tempresult)')>0;
    end
    
end


