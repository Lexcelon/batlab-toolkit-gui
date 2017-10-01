nCells = 600;
capacities = 3.3 + randn(nCells,1)*.05;
ESRs = .040 + randn(nCells,1)*.003 - .03*(capacities-mean(capacities))  ;
scatter(capacities,ESRs,4,'filled');
title('Input Cell Parameters');
xlabel('Capacity (AH)');
ylabel('ESR (mO)');


cellData = repmat(struct('capacity',[],'ESR',[],'ID',[]),nCells,1)

for(i=1:nCells)

    [cellData(i).capacity] = deal(capacities(i)); %store in cell propertie
    [cellData(i).ESR] = deal(ESRs(i)); %store in cell propertie
    [cellData(i).ID] = deal(i) ;%store in cell propertie
    cellData(i).voltageLowLimit = 2.5;
    cellData(i).voltageHighLimit = 4.2;
    cellData(i).chargeCurrentLimit=3.4;
    cellData(i).dischargeCurrentLimit=10;

end;
