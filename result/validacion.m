clear;

load signal.txt;
load result.txt;
load H1.mat;
filter = H_1;

%tic
resultConv = conv(signal, filter);
%toc

value = zeros(2,length(result));

for i = 1:length(result)
    value(1,i) = abs(result(i,2)) - abs(resultConv(i));
    value(2,i) = abs(value(1,i)) / abs(resultConv(i));
end

value = value';


resultadofinal = zeros(4,length(result));

for i = 1:length(result)
    resultadofinal(1,i) = resultConv(i);
    resultadofinal(2,i) = result(i,2);
    resultadofinal(3,i) = value(i,1);
    resultadofinal(4,i) = value(i,2);
end
resultadofinal = resultadofinal';
