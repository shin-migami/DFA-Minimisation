//Originally written by Suvojit Manna
//Edited by Somnath Rakshit

//edited by Susmita Dutta#include<stdio.h>

#define MAX_STATE 26
#define MAX_PART 10

typedef struct stateInfo
{
    int out[2],nState[2],groupNo;
    char pState,nextState[2];
}state;

typedef struct partitionTable
{
    int groupAssign[MAX_STATE];
    int groupMax,groupMin,groupCount;
}partTable;

state stateTable[MAX_STATE];
partTable groupChart[MAX_PART];

void assign_state(int stateCount)
{
    int i,j;
    int flag0 = 0,flag1 = 0;
    for(i=0; i < stateCount; ++i)
    {
        for(j=0; j < stateCount; ++j)
        {
            if(!flag0 && stateTable[i].nextState[0] == stateTable[j].pState)
            {
                stateTable[i].nState[0] = j;
                flag0 = 1;
            }
            if(!flag1 && stateTable[i].nextState[1] == stateTable[j].pState)
            {
                stateTable[i].nState[1] = j;
                flag1 = 1;
            }
        }
        flag0 = 0;
        flag1 = 0;
    }
}

void input_state_table(int *stateCount)
{
    int i;
    char cState,nState0,nState1,out0,out1;

    printf("Enter No of States : ");
    scanf("%d\n",stateCount);
    printf("Enter space separated values in format \
        \nCurrent State  Next State(x=0)  Output(x=0)  Next State(x=1)  Output(x=1)\
        \n    (char)         (char)          (int)         (char)         (int)\n");
    for(i=0; i<*stateCount; ++i)
    {
        fflush(stdin);
        scanf("%c %c %c %c %c\n",&cState,&nState0,&out0,&nState1,&out1);
        stateTable[i].pState = cState;
        stateTable[i].nextState[0] = nState0;
        stateTable[i].nextState[1] = nState1;
        stateTable[i].out[0] = out0 - '0';
        stateTable[i].out[1] = out1 - '0';
        stateTable[i].groupNo = 0;
    }
    assign_state(*stateCount);
}

void print_state_table_private(int stateCount)
{
    int i;
    for(i=0; i<stateCount; ++i)
    {
        printf("%c(S%d) %c(S%d) %d %c(S%d) %d\n",
               stateTable[i].pState,i,
               stateTable[i].nextState[0],stateTable[i].nState[0],
               stateTable[i].out[0],
               stateTable[i].nextState[1],stateTable[i].nState[1],
               stateTable[i].out[1]);
    }
}

void print_state_table(int stateCount)
{
    int i;
    for(i=0; i<stateCount; ++i)
    {
        printf("%c %c %d %c %d\n",
               stateTable[i].pState,
               stateTable[i].nextState[0],
               stateTable[i].out[0],
               stateTable[i].nextState[1],
               stateTable[i].out[1]);
    }
}

void initial_partition(int stateCount)
{
    //Finalize the P1 partition
    int i,setValueFlag = 0,findGroupMin,x0,x1;
    for(i=0; i<stateCount; ++i)
    {
        groupChart[0].groupAssign[i] = 0;
    }
    groupChart[0].groupMax = groupChart[0].groupMin = 0;
    groupChart[0].groupCount = 1;
    //Start Preparing the P1 Partition
    groupChart[1].groupMax = groupChart[1].groupMin = 0;
    groupChart[1].groupCount = 1;
    for(i=0; i<stateCount; ++i)
    {
        //Copy Group Assignment from Previous Partition becomes Default for 00
        groupChart[1].groupAssign[i] = groupChart[0].groupAssign[i];
    }
    for(x0 = 0;x0 < 2; ++x0)
    {
        for(x1 = 0;x1 < 2; ++x1)
        {
            for(i=0; i<stateCount; ++i)
            {
                //For 01
                if(stateTable[i].out[x0] == 0 && stateTable[i].out[x1] == 1)
                {
                    if(setValueFlag)
                        //if the group is already made
                        groupChart[1].groupAssign[i] = groupChart[1].groupMax;
                    else
                    {
                        //make new group and update variables
                        groupChart[1].groupAssign[i] = groupChart[1].groupMax + 1;
                        setValueFlag = 1;
                        groupChart[1].groupMax++;
                        groupChart[1].groupCount++;
                    }
                }
            }
        }
    }
    //Adjust Group Names
    findGroupMin = groupChart[1].groupMax;
    for(i=0; i<stateCount; ++i)
    {
        if(groupChart[1].groupAssign[i] < findGroupMin)
            findGroupMin = groupChart[1].groupAssign[i];
    }
    if(findGroupMin > 0)
    {
        for(i=0; i<stateCount; ++i)
            groupChart[1].groupAssign[i]--;
        groupChart[1].groupMax--;
        groupChart[1].groupCount = groupChart[1].groupMax - groupChart[1].groupMin + 1;
    }
}

int equivalent(int partIndex)
{
    if(groupChart[partIndex].groupCount != groupChart[partIndex - 1].groupCount)
        return 0;
    else
        return 1;
}

int successive_partition(int stateCount)
{
    int i=1,k,l,m,setValueFlag = 0,stateI;
    while(!equivalent(i))
    {
        //Set to Partition 2
        ++i;
        //Copy Data from Prev Partition
        groupChart[i].groupMax = groupChart[i-1].groupMax;
        groupChart[i].groupMin = groupChart[i-1].groupMin;
        groupChart[i].groupCount = groupChart[i-1].groupCount;
        for(k=0; k<stateCount; ++k)
        {
            //Copy Group Assignment from Previous Partition
            groupChart[i].groupAssign[k] = groupChart[i-1].groupAssign[k];
        }
        //Latest Try
        for(stateI = 0; stateI <= 1; ++stateI)
        {
            #ifdef DEBUG
                printf("State = %d\n",stateI);
            #endif // DEBUG
            for(k = groupChart[i].groupMin; k <= groupChart[i].groupMax; ++k)
            {
                #ifdef DEBUG
                    printf("  Inspected Group = %d\n",k);
                #endif // DEBUG
                for(l = 0;l < stateCount; ++l)
                {
                    if(groupChart[i].groupAssign[l] == k)
                    {
                        #ifdef DEBUG
                            printf("    Check state = %c\n",stateTable[l].pState);
                        #endif // DEBUG
                        for(m = l+1; m < stateCount; ++m)
                        {
                            if(groupChart[i].groupAssign[m] == k)
                            {
                                #ifdef DEBUG
                                    printf("      With State = %c ",stateTable[m].pState);
                                    printf("Next State : %c(%d) & %c(%d)\n",
                                           stateTable[l].nextState[stateI],
                                           groupChart[i-1].groupAssign[stateTable[l].nState[stateI]],
                                           stateTable[m].nextState[stateI],
                                           groupChart[i-1].groupAssign[stateTable[m].nState[stateI]]);
                                #endif // DEBUG
                                if(groupChart[i-1].groupAssign[stateTable[l].nState[stateI]] != groupChart[i-1].groupAssign[stateTable[m].nState[stateI]])
                                {
                                    if(setValueFlag == 0)
                                    {
                                        setValueFlag = 1;
                                        ++groupChart[i].groupCount;
                                        ++groupChart[i].groupMax;
                                    }
    //commentline1                                groupChart[i].groupAssign[m] = groupChart[i].groupMax;
                                }
                            }
                        }
                        setValueFlag = 0;
                    }
                }
            }
        }
    }
    return i;
}

void print_groups(int finalPartition,int stateCount)
{
    int partNo,stateNo,grpNo;
    printf("\n");
    for(partNo = 0; partNo < finalPartition; ++partNo)
    {
        printf("P%d = ",partNo);
        for(grpNo = 0; grpNo <= groupChart[partNo].groupMax; ++grpNo)
        {
            printf("(");
            for(stateNo = 0;stateNo < stateCount; ++stateNo)
            {
                if(groupChart[partNo].groupAssign[stateNo] == grpNo)
                {
                    printf("%c ",stateTable[stateNo].pState);
                }
            }
            printf("\b)");
        }
        printf("\n");
    }
}

void state_partition(int stateCount)
{
    int finalPartition;
    initial_partition(stateCount);
    finalPartition = successive_partition(stateCount);
    print_groups(finalPartition,stateCount);
}

int main()
{
    int stateCount;
    input_state_table(&stateCount);
    #ifdef DEBUG
        print_state_table_private(stateCount);
    #endif // DEBUG
    state_partition(stateCount);
    return 0;
}
