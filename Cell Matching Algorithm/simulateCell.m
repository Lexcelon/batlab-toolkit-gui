function [terminalVoltage, current,SoCExpended] = simulateCell(thisCellInfo,loadCurveTime,loadCurvePower,cellDataTable,startSoC, endSoC )

cellDataTable.capacityData = cellDataTable.capacityData * thisCellInfo.capacity * 3600; %scale for this cell and convert to amp-seconds

%preallocate
terminalVoltage = zeros(size(loadCurveTime));
current=zeros(size(loadCurveTime));
capacityExpendedAS = zeros(size(loadCurveTime));
SoCExpended  = zeros(size(loadCurveTime));

dt = diff(loadCurveTime);
VUL = interp1(cellDataTable.capacityData,cellDataTable.voltageData,capacityExpendedAS(1),'linear','extrap');% first time init only
capacityExpendedAS(1) = (1-startSoC) .* thisCellInfo.capacity;

VOCfit = fit(cellDataTable.capacityData,cellDataTable.voltageData,'poly1');

for(i=1:size(loadCurveTime,1)-1) %run the simulation
    %VOC = interp1(cellDataTable.capacityData,cellDataTable.voltageData,capacityExpendedAS(i),'linear','extrap');
    VOC = feval(VOCfit,capacityExpendedAS(i));  %This bulit in implementation is terribly slow
    current(i) = loadCurvePower(i) / (VUL);
    %current(i) = ( sqrt(VOC^2-4*loadCurvePower(i)*thisCellInfo.ESR) + VOC ) / (2*thisCellInfo.ESR); %solved quadratic, r can't be zero
    %current(i) = (sqrt(VOC^2-4*loadCurvePower(i)*thisCellInfo.ESR) + VOC)/(2*thisCellInfo.ESR);

    
    if(current(i)>thisCellInfo.dischargeCurrentLimit || current(i)<-thisCellInfo.chargeCurrentLimit || VOC>thisCellInfo.voltageHighLimit || VOC<thisCellInfo.voltageLowLimit)
        break;
    end;
    
    if(SoCExpended(i)>1-endSoC)
        break;
    end;
    
    terminalVoltage(i) = VOC-thisCellInfo.ESR * current(i);
    VUL = terminalVoltage(i);
    capacityExpendedAS(i+1) = capacityExpendedAS(i) + current(i)*dt(i); %super simple integration
    SoCExpended(i) = capacityExpendedAS(i)/thisCellInfo.capacity/3600;

end;

