import ROOT
import pandas as pd

#prepare datasets
N=10. # N x N
L=285. # mm
gap = L/N
LE = -0.5*L

def initialize_data():
    data = {
    }
    for l in range(3):
        for i in range(int(N*N)):
            data["t_"+str(l)+"_"+str(i)]=[]
    for i in range(25):
        data["ecal_"+str(i)]=[]
    data["conv_e"]=[]
    data["isconv"]=[]
    data["conve_inECAL"]=[]
    data["convp_inECAL"]=[]
    data["reco_ratio"]=[]
    data["init_E"]=[]

    return data
def create_entry(data):
    data["conv_e"].append(0)
    data["isconv"].append(0)
    data["conve_inECAL"].append(0)
    data["convp_inECAL"].append(0)
    data["reco_ratio"].append(0)
    data["init_E"].append(0)
    for i in range(25):
        data["ecal_"+str(i)].append(0)
    for i in range(int(N*N)):
        data["t_0_"+str(i)].append(0)
        data["t_1_"+str(i)].append(0)
        data["t_2_"+str(i)].append(0)
    

data = initialize_data()

file = ROOT.TFile.Open("test.root")
tree = file.Get("vtree")
if not tree:
    print("Error: TTree 'vtree' not found in the file.")
    exit(1)

index=0
for event in tree:
    if(index%10000==0):
        df = pd.DataFrame(data)
        df.to_pickle("data_"+str(index)+".pkl")
        print(index)
        data = initialize_data()
    create_entry(data)
    data["conv_e"][-1] = event.conv_e
    data["isconv"][-1] = event.isconv
    data["conve_inECAL"][-1] = event.conve_inECAL
    data["convp_inECAL"][-1] = event.convp_inECAL
    data["init_E"][-1] = event.init_E
    ecal_e = 0.
    
    for i in range(len(event.ecal_cellid)):
        id = event.ecal_cellid[i]
        e = event.ecal_celle[i]
        ecal_e += e
        data["ecal_"+str(id)][-1] = e
    data["reco_ratio"][-1] = (event.conv_e + ecal_e)/event.init_E
    for i in range(len(event.tracker_hitx)):
        x = event.tracker_hitx[i]
        y = event.tracker_hity[i]
        xyid = int((x-LE)/gap)*int(N) + int((y-LE)/gap)
        z = int(event.tracker_hitz[i])
        if(z==-215):
            data["t_0_"+str(xyid)][-1]+=1
        elif(z==-184):
            data["t_1_"+str(xyid)][-1]+=1
        elif(z==-153):
            data["t_2_"+str(xyid)][-1]+=1
    index+=1

df = pd.DataFrame(data)
df.to_pickle("data_"+str(index)+".pkl")
# print(df)
    # print(data)
    # print(event.tracker_hitx)
