import imageio
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import os

def img_file_to_gif(img_files, output_file_name, dur):
    if not os.path.exists("../Vaccin/PlotFile/GIF/"):
        os.makedirs("../Vaccin/PlotFile/GIF/")
    imgs_array = [np.array(imageio.imread(img_file)) for img_file in img_file_lst]
    imageio.mimsave(output_file_name, imgs_array, duration=dur)


option = int(input("생성할 파일 옵션\n"
    "1.By_Times_Immune: 면역 생성자 변화(animated)\n"
    "2.By_Times_Infect_Rate: 시행별 감염률(image)\n"
    "3.By_Times_Infection: 감염자 변화(animated)\n"
    "4.Immune_Accu: 누적 면역자(image)\n"
    "5.Infection_Accu: 누적 감염자(image)\n"
    "Option(num): "))

filename_option = None

if option == 1:
    filename_option = "By_Times_Immune"
elif option == 2:
    filename_option = "By_Times_Infect_Rate"
elif option == 3:
    filename_option = "By_Times_Infection"
elif option == 4:
    filename_option = "Immune_Accu"
elif option == 5:
    filename_option = "Infection_Accu"

COL = int(input('\nfield 크기를 입력하세요\n세로: '))
filename = None
picture_name = None
gif_name = None
img_file_lst = []
if option == 1 or option == 3:
    Days = int(input("일차수를 입력하세요: "))
    Times = int(input("시행수를 입력하세요: "))
    Vaccin_Rate = int(input("접종률을 입력하세요(0<=n<=100): "))
    dur = float(input("재생속도를 입력하세요(default:0.5): "))

    for i in range(Times):
        filename = \
            "/Users/choidaehyun/Documents/GitHub/Vaccin/CalResult/"+filename_option+"-"+repr(Vaccin_Rate)+"-"+repr(i+1)+".txt"
        data = np.loadtxt(
            filename,
            delimiter=',',
            dtype='int'
        )
        for j in range(Days):
            print("시행"+repr(i+1)+"/"+repr(round(j/Days*100,2))+"%완료\n")
            start = j*COL
            end = (j+1)*COL
            sns.heatmap(data[start:end])
            if option == 1:
                picture_name = "../Vaccin/PlotFile/By_Times_Immune_"+repr(i+1)+"_"+repr(j+1).zfill(4)
            elif option == 3:
                picture_name = "../Vaccin/PlotFile/By_Times_Infection_"+repr(i+1)+"_"+repr(j+1).zfill(4)
          # FileName_Time_Day
            plt.savefig(picture_name)
            img_file_lst.append(picture_name+".png")
            plt.close()

        if option == 1:
            gif_name = "../Vaccin/PlotFile/GIF/By_Times_Immune_"+repr(i+1)+".gif"
        elif option == 3:
            gif_name = "../Vaccin/PlotFile/GIF/By_Times_Infection_"+repr(i+1)+".gif"
        img_file_to_gif(img_file_lst,gif_name,dur)
        img_file_lst.clear()
elif option == 2 or option == 4 or option == 5:
    Times = int(input("시행수를 입력하세요: "))
    Vaccin_Rate = int(input("접종률을 입력하세요(0<=n<=100): "))

    for i in range(Times):
        filename = \
            "/Users/choidaehyun/Documents/GitHub/Vaccin/CalResult/" + filename_option + "-" + repr(
                Vaccin_Rate) + "-" + repr(i + 1) + ".txt"
        data = np.loadtxt(
            filename,
            delimiter=',',
            dtype='int'
        )
        if option == 2:
            picture_name = "../Vaccin/PlotFile/By_Times_Infect_Rate_"
        elif option == 4:
            picture_name = "../Vaccin/PlotFile/Immune_Accu_"
        elif option == 5:
            picture_name = "../Vaccin/PlotFile/Infection_Accu_"
        sns.heatmap(data)
        # Filename_Time
        plt.savefig(picture_name + repr(i + 1))
        plt.close()
