function [T,Tt] = D_Kin( Robot )

%   T  -> Transformation Array
%   Tt -> Transformation from 0 to 7

n=size(Robot,1);      %get number of robot joints

Tt=eye(4);


for i=1:n
    
    row=Robot(i,:);
    
    %Transformation Matrix
    %From robotics book
    T{i}=[cos(row(2)+row(5)) -sin(row(2)+row(5))*cos(row(4)) sin(row(2)+row(5))*sin(row(4))   row(3)*cos(row(2)+row(5));
          sin(row(2)+row(5)) cos(row(2)+row(5))*cos(row(4))  -cos(row(2)+row(5))*sin(row(4))  row(3)*sin(row(2)+row(5));
          0                  sin(row(4))                     cos(row(4))                      row(1)                   ;
          0                  0                               0                                1                        ];
    
    Tt=Tt*T{i};
end


%Simplify
Tt=simplify(Tt);

for i=1:n
    T{i}=simplify(T{i});
end


end