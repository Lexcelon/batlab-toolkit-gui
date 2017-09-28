clc;clear;close all;

synthesizeData;

nModules = 3;
nCellsInModule=2;
nominalCellVoltage = 3.65;

nCellsInPack = nModules*nCellsInModule;
roughGuessPackCapacityWH = nominalCellVoltage * nCellsInPack * mean([cellData(:).capacity]);

load('dataForHayden.mat');

[cellCurve.voltageData,locs]=findpeaks(dataForHayden(1).dischargeData(:,1),'MinPeakProminence',.05); %  find the OCV
cellCurve.capacityData = dataForHayden(1).dischargeData(locs,7);
cellCurve.capacityData = cellCurve.capacityData./ max(cellCurve.capacityData); %normalize to capacity of 1.
% maybe extract this curve from level 1 data and average? The curve may well also be useful to them.

%% Prepare the simulation test curve
peakPower = 50; %watts, from user?
repeatOrScaleInTime = 'repeat';
[inData] = xlsread('NewYorkCitySchedule.csv'); %Perhaps let them pick a CSV file (this file is speed, not power or acceleration, but whatever)
time = inData(:,1);
power = inData(:,2);

power = power * (peakPower./max(power)); %Scale input for peak power

inputScheduleEnergy = cumsum( diff(time) .* power(2:end)) / 3600; 

if(strcmp(repeatOrScaleInTime, 'scale'))
    time = time*roughGuessPackCapacityWH/inputScheduleEnergy(end); %stretch out time
elseif(strcmp(repeatOrScaleInTime, 'repeat'))
    nRepeats = ceil(roughGuessPackCapacityWH/inputScheduleEnergy(end));
    dt = diff(time);
    time = cumsum(kron([mean(dt); dt],ones(nRepeats,1)));
    power = kron(power,ones(nRepeats,1));
end;

%Somethign to resample their input to a reasonable number (300-1000) time
%steps would be good. Maybe prompt them for a combined time stretch and
%repeat

startingSoC = 1- .05; % 95% to 5%
endingSoC = .05;

% constantPowerDischargeTime = .98 * roughGuessPackCapacityWH / constantPower * 3600; %simulated discharge duration in seconds
% constPowerCurveTimebase = 0:10:constantPowerDischargeTime;                          %Could also pull this from files, must figure out how to scale to their application
% constantPowerCurve = constantPower*ones(size(constPowerCurveTimebase));

theDischargeTimebase = time;
theDischargePowerCurve = power /nCellsInPack ;   

nCells = 10; % for development
%% Do the simulation
for(i=1:nCells)
    [cellData(i).simTerminalVoltage, cellData(i).simCurrent, cellData(i).simSoC] = ...
      simulateCell(cellData(i),theDischargeTimebase,theDischargePowerCurve,cellCurve,startingSoC,endingSoC);
    fprintf('Simulating cell %d\n', i)
end;

%compute error matrices:
%Preallocate
errorVoltage = zeros(nCells);
errorCurrent = zeros(nCells);
errorSoC = zeros(nCells);


%% Compute symetric matrices of RMS differences between cells
for(i=1:nCells);
    for(j=1:i)
        errorVoltage(i,j) = sum( (cellData(i).simTerminalVoltage - cellData(j).simTerminalVoltage).^2 );
        errorVoltage(j,i) = errorVoltage(i,j);
        errorVoltage(i,i) = 0;
        
        errorCurrent(i,j) = sum( (cellData(i).simCurrent - cellData(j).simCurrent).^2 );
        errorCurrent(j,i) = errorCurrent(i,j);
        errorCurrent(i,i) = 0;
        
         errorSoC(i,j) = sum( (cellData(i).simSoC - cellData(j).simSoC).^2 );
        errorSoC(j,i) = errorSoC(i,j);
        errorSoC(i,i) = 0;
    end;
end;

scalarErrorMetric = 1 *  errorVoltage + 1 * errorCurrent + 1* errorSoC; %Arbitrary linear combination of errors to form a scalar

%% Diagnostic Figures
figure;
plot([cellData(1:10).capacity]) 
title('Capacity vs cell index')

figure;
subplot(3,1,1);
 plot(cellData(9).simTerminalVoltage)
hold all
plot(cellData(10).simTerminalVoltage)
title('Terminal Voltage vs cell index')

subplot(3,1,2);
 plot(cellData(9).simCurrent)
hold all
plot(cellData(10).simCurrent)
title ('Current vs time');

subplot(3,1,3);
 plot(cellData(9).simSoC)
hold all
plot(cellData(10).simSoC)
title ('SOC vs time');




%% group cells into modules

cellScores = mean(scalarErrorMetric,1); %Average provides some normalization
[~,ind] = sort(cellScores,'ascend'); % put worst cells last - the worst cell that we must use gets first pick of other cells
[~,cellPreferenceIndices] = sort(scalarErrorMetric,1,'ascend'); %now each column contains an orded list of cells most simliar to that cell
cellsLeftToPutInPack = nCellsInPack;
startingIndex = cellsLeftToPutInPack ; %this is an index into ind, above, coresponding to the worst cell we think we will include in the pack

cellsInModule = zeros(nModules,nCellsInModule); 
availableCells = ind;
cellToMatch = availableCells(startingIndex);


cCellPreferenceIndices = mat2cell(cellPreferenceIndices,nCells,ones(nCells,1)); %% Cells play nicer with not being square
for(i=1:nModules)
   
        
        cellsInModule(i,:) =cCellPreferenceIndices{cellToMatch}(1:nCellsInModule);
       %Clean up cellPreferenceIndices 
       for(k=1:size(cCellPreferenceIndices,2))
           for(j=1:nCellsInModule)
               cellToRemove = cellsInModule(i,j) ;
               cCellPreferenceIndices{k}(cCellPreferenceIndices{k} == cellToRemove) = [];  %remove the cell 
                                       %from the (queue) of preferred cells for further selection - 
                                       %since we can't have it in the pack more than once
              if(k==1)
                  availableCells(availableCells==cellToRemove) = []; % also remove from list of available cells
              end;
           end;
       end;
        
  
    cellsLeftToPutInPack = cellsLeftToPutInPack-nCellsInModule;
    if(cellsLeftToPutInPack>0)
        cellIndToMatch = cellsLeftToPutInPack;  %The worst cell that we think we will need to include
        cellToMatch = availableCells(cellIndToMatch);
    end;
    
end;





