#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;
struct endpointInfo
{
    int cacheId;
    int latencty;
};
struct endpointDescription
{
    int nrOfCaches;
    vector<endpointInfo> endpointConnections;
};
struct requestsDescription
{
    int videoId;
    int endpointId;
    int nrRequests;
};
int nrVideos, nrEndpoints, reqDesc, nrCached, cacheCapacity;
int *videosWeight, *sortedVideosByRequests;
vector<endpointDescription> allEndpoints;
vector<requestsDescription> allRequests;



bool getDataIntoMemory()
{
    fstream f;
    f.open("videos_worth_spreading.in", ios::in);
    if(!f){
        f.close();
        f.clear();
        return false;
    }
    f>>nrVideos;
    f>>nrEndpoints;
    f>>reqDesc;
    f>>nrCached;
    f>>cacheCapacity;
    //general info done
    videosWeight = new int[nrVideos];
    sortedVideosByRequests = new int[nrVideos+1];
    for(int i=0;i<nrVideos;i++)
        f>>videosWeight[i];
    //videos weight done

    for(int i=0;i<nrEndpoints;i++){
        int tmpEndpointDatacenterLatency, tmpNumberOfCaches;
        f>>tmpEndpointDatacenterLatency;
        f>>tmpNumberOfCaches;
        endpointInfo tmpEndpointInfo;// = new endpointInfo;
        tmpEndpointInfo.cacheId = -1;
        tmpEndpointInfo.latencty = tmpEndpointDatacenterLatency;
        endpointDescription tmpEndpointDescription;
        tmpEndpointDescription.nrOfCaches = tmpNumberOfCaches+1;
        tmpEndpointDescription.endpointConnections.push_back(tmpEndpointInfo);
        for(int j=0;j<tmpNumberOfCaches;j++){
            f>>tmpEndpointInfo.cacheId;
            f>>tmpEndpointInfo.latencty;
            tmpEndpointDescription.endpointConnections.push_back(tmpEndpointInfo);
        }
        allEndpoints.push_back(tmpEndpointDescription);
    }
    for(int i=0;i<reqDesc;i++){
        requestsDescription tmpRequestDescription;
        f>>tmpRequestDescription.videoId;
        f>>tmpRequestDescription.endpointId;
        f>>tmpRequestDescription.nrRequests;
        allRequests.push_back(tmpRequestDescription);
    }
    for(int i=0;i<nrVideos+1;i++)
        sortedVideosByRequests[i]=0;
    for(int i=0;i<reqDesc;i++){
        sortedVideosByRequests[allRequests[i].videoId] += allRequests[i].nrRequests;
    }


    f.close();
    f.clear();
    return true;
}
void clearMemory()
{
    delete videosWeight;
    delete sortedVideosByRequests;
}
void dumbSolve()
{
    fstream fout;
    fout.open("videos_worth_spreading.out22", ios::out);
    int *sorted2 = new int[nrVideos+1];
    vector<int> videosIdBySize;

    for(int i=0;i<nrVideos+1;i++)
        sorted2[i] = sortedVideosByRequests[i];
    sort(sorted2, sorted2+nrVideos+1, std::greater<int>());
    for(int i=0;i<nrVideos+1;i++)
    for(int j=0;j<nrVideos+1;j++){
        if(sortedVideosByRequests[j] == sorted2[i])
            videosIdBySize.push_back(j);
    }
    vector<int> stuff;
    int bla = 0, k=0;
    while(true){
        bla+=videosWeight[videosIdBySize[k]];
        if(bla > cacheCapacity){
            bla-=videosWeight[videosIdBySize[k]];
            goto WHILEEND;
        }
        stuff.push_back(videosIdBySize[k]);
        k++;
    }

    WHILEEND:
    fout<<nrCached<<endl;
    for(int i=0;i<nrCached;i++){
        fout<<i;
        for(int j=0;j<stuff.size();j++)
            fout<<" "<<stuff[j];
        fout<<endl;
    }

    delete sorted2;
    fout.close();
    fout.clear();
}
bool ifHasCacheId(endpointDescription om, int cacheId)
{
    for(int i=0;i<om.endpointConnections.size();i++){
        if(om.endpointConnections[i].cacheId == cacheId)
            return true;
    }
    return false;
}
void smartSolve()
{
    fstream fout;
    fout.open("videos_worth_spreading.out222", ios::out);
    fout<<nrCached<<endl;

    for(int cacheId = 0;cacheId<nrCached;cacheId++){
        cout<<cacheId<<"/"<<nrCached<<endl;

       map<int,int> punctajVideos;


        //int *bla1 = new int[nrVideos+1];
        for(int j=0;j<nrVideos+1;j++)
            punctajVideos[j]=0;
        for(int i=0;i<allEndpoints.size();i++){
            if(ifHasCacheId(allEndpoints[i], cacheId)){
                for(int j = 0; j<allRequests.size();j++){
                    if(allRequests[j].endpointId == i){
                        punctajVideos[allRequests[j].videoId] += allRequests[j].nrRequests;
                    }
                }
            }
        }
        vector<std::pair<int,int>> items;
        for(int i=0;i<nrVideos;i++)
            items.push_back(pair<int,int>(punctajVideos[i], i));
        sort(items.begin(), items.end());
        int currentLoad = 0;
        fout<<cacheId;
        for(int i=items.size()-1;i>=0;i--){
            pair<int,int> tmppair = items[i];
            currentLoad += videosWeight[tmppair.second];
            if(currentLoad > cacheCapacity){
                goto FOREND;
            }
            fout<<" "<<tmppair.second;
        }
        FOREND:
        fout<<endl;
    }
}

int main()
{
    if(!getDataIntoMemory()){
        cout<<"No file"<<endl;
        return 0;
    }
    smartSolve();
    clearMemory();

    return 0;
}
