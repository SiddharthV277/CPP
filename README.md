# CPP
This repository is for all c++ projects
1. savefile: this program ensures that your important files that you have saved in your local drive stays secure from accidental deletion.
             The way it works is quite simple, it works by readunf filename, any file that contains word "imp/important" then transfers the
             file to a secure location that is impervious to accidental deletion, for example: C: Drive.
             The word filtering method is immature for the time being, it only works how the dev describes it . For instance , a filename accidentaly
             written as "Importamt" instead of "Important". Program won't detect it and hence won't copy it to the destination path.
             Later updats will also include cloud transferring.
