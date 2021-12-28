function result = SoundRecord(time)

recorder = audiorecorder(44100,16,1);
record(recorder,time);
recordblocking(recorder,time); %녹음 도중에 스크립트가 진행되지 않도록 hold
result = getaudiodata(recorder);
