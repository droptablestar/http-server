import os,math

def main():
    dirs = ["t1_24ms_10_col",  "t1_24ms_11_in",  "t1_6ms_11_1M",
            "t1_24ms_10_in",   "t1_6ms_10_1M",   "t1_6ms_11_col",
            "t1_24ms_11_1M",   "t1_6ms_10_col",  "t1_6ms_11_in",
            "t1_24ms_10_1M",  "t1_24ms_11_col"]

    nums = []
    for dir0 in dirs:
        for dir1 in os.listdir(dir0):
            path = dir0+"/"+dir1+'/'
            path = 't1_6ms_10_in/50/'
            files = os.listdir(path)
            print path
            for f in files:
                with open(path+f) as fi:
                    lines = fi.readlines()
                for line in lines:
                    line = line.strip('\n').split(',')
                    line = map(float, line)
                    nums.append(line)
        
            stats(nums,path)
            exit()
def join():
    with open('results/total.txt','w') as f:
        for n in os.listdir('results'):
            if n != 'total.txt':
                with open('results/'+n) as f0:
                    f.write(f0.read())
                            
def stats(nums,name):
    sum_c, sum_t, sum_f = 0,0,0
    for line in nums:
        sum_c += line[0]
        sum_t += line[1]
        sum_f += line[2]

    avg_c = sum_c/len(nums)
    avg_t = sum_t/len(nums)
    avg_f = sum_f/len(nums)

    stds = std(nums,(avg_c,avg_t,avg_f))
    print stds
    print avg_c, avg_t, avg_f
    name = 'results/'+name.replace('/','_').replace('t1','').strip('_')+'.txt'
    with open(name,'w') as f:
        f.write('%.3f\t%.3f\t%d\t%.3f\t%.3f\t%f\t%s\n' %
                (avg_c,avg_t,avg_f,stds[0],stds[1],stds[2],name))
    
    
def std(nums, avg):
    std_c, std_t, std_f = 0, 0, 0
    for line in nums:
        std_c += (line[0] - avg[0])**2
        std_t += (line[1] - avg[1])**2
        std_f += (line[2] - avg[2])**2

    std_c = math.sqrt((1.0/(len(nums)-1))*std_c)
    std_t = math.sqrt((1.0/(len(nums)-1))*std_t)
    std_f = math.sqrt((1.0/(len(nums)-1))*std_f)

    return std_c,std_t,std_f
    
if __name__ == '__main__':
    main()
