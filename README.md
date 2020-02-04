# S3 EPITA Project - Optical Character Recognition
This project has been made by students of EPITA in third semester (second year).
The goal of this project is to recognize the text which is on a picture and return it.



## Members of the team
Here is the members of the team :

- BLANCHETON Tao
- DU MESNIL DU BUISSON Geoffroy
- SOUYRI Tanguy
- WATTELET Mathieu



## How to compile the project ?
Open a linux terminal in the root of the git repository.
Execute the command "make interface" to compile the project.



## How to use ?
After compiling the project, execute the command "./interface" to launch the project.
Chose a PNG file and a download folder. To finish, clic on the download button.



## Dependencies
You have the following packages on your computer to compile and launch the project :

- GTK 3
- SDL 2



## DEV
If you want to see the results of the segmentation on the "test.png" image, go to the folder named "DEV".
Launch a linux terminal and execute the commande "make segmentation".
Execute the command "./main_seg 1".

The folders "Para", "Ligne", "Word" and "Char" appear. 
These folders contain the segmented block (BMP image).
Moreover, the file "text.txt" appear and contain the recognized text of the picture.


To clean all that, execute the command "make clean_seg".
