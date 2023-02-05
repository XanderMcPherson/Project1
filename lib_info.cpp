/*Xander McPherson, Andrew Eblen  2/4/2023 lib_info.cpp
  This program takes in info and stores it lexographically into maps and prints out in the desired format*/

#include <iostream>
#include <map>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

struct Song { 
    string title;
	string genre;
    int time;
};

struct Album {
    map <int, Song > songs;
    string name;
    int time;
    int nsongs;  // optional variable but makes it easier
};

struct Artist {
    map <string, Album > albums;
    string name;
    int time;
    int nsongs;
};

int convert (string time) {
	int mins = 0;
	int secs = 0;
	int total;
	
	char nTime[time.size()];

	for (int i = 0; i < time.size(); i++) {
		nTime[i] = time[i];
	}

	sscanf(nTime, "%d:%d", &mins, &secs);
	
	total = ((mins * 60) + secs);

	return total;
}

/*This function is called when the time is read in from the file as a string. It converts this normal time format into total
seconds so the code can keep a running total of the time per artist and album */

string convertback (int time) {
    int minutes, seconds;
    string mins, secs, total;
    ostringstream ss1;
    ostringstream ss2;

    minutes = time/60;
    seconds = time%60;
    ss1 << minutes;
    ss2 << seconds;
    mins = ss1.str();
    secs = ss2.str();

    if(seconds < 10) {
        secs = "0" + secs;
    }
    total = mins + ":" + secs;

    return total;
}

/*This is called at the very end to convert those total seconds back into the XX:XX time format. */


int main(int argc, char *argv[]) {
	ifstream fin;
	fin.open(argv[1]);
	string titl, time, artist, album, genre;
	int track;
	int secs;
	int check;
	int aCheck;
	map <string, Artist> artists;
	map <string, Artist>::iterator it = artists.begin();
	map <string, Album>::iterator it2;
	map <int, Song>::iterator it3;
	while (fin >> titl >> time >> artist >> album >> genre >> track) {
		it = artists.begin();
		check = 0;
		aCheck = 0;
		for (int i = 0; i < titl.size(); i++) {
			if (titl[i] == '_') {
				titl[i] = ' ';
			}
		}
		secs = convert(time);
		for (int i = 0; i < artist.size(); i++) {
			if (artist[i] == '_') {
				artist[i] = ' ';
			}
		}
		for (int i = 0; i < album.size(); i++) {
			if (album[i] == '_') {
				album[i] = ' ';
			}
		}
		for (it = artists.begin(); it != artists.end(); ++it) {
            if (it->first == artist) {
                check++;
            }
        }	
		if (check == 0) {
			artists[artist] = Artist();
		}

		/* Above removes all underscores and changes them to spaces. Iterates through the artists map to see 
		if the artist has been seen before. If it hasnt been seen it stores the artist name into the map with an 
		instance of Artist */

		for (it = artists.begin(); it != artists.end(); ++it) {
			if (it->first == artist) {
				break;
			}
		}
		if (check == 0) {
			it->second.time = 0;
			it->second.albums[album] = Album();
			it2 = it->second.albums.begin();

			for (it2 = it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {
				if (it2->first == album) {
					break;
				}
			}
			it2->second.name = album;
			it2->second.time = 0;
			it2->second.nsongs = 0;
			it2->second.time += secs;
			it2->second.songs[track] = Song();
			it2->second.nsongs++;
			for (it3 = it2->second.songs.begin(); it3 != it2->second.songs.end(); ++it3) {
				if (it3->first == track) {
					break;
				}
			}
			it3->second.title = titl;
			it3->second.time = secs;
		}
		/* This is code specifically for storing if the artist has not been seen before. Since the artist 
		has not been seen we know that none of the albums or songs have not been stored either so we can store these
		things with ease. I have three total iterators to move between all the maps. */
		else {
			for (it2 = it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {
				if (it2->first == album) {
					aCheck++;
				}
			}
			if (aCheck == 0) {
				it->second.albums[album] = Album();
				for (it2 = it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {
                	if (it2->first == album) {
                    	break;
                	}
            	}
				it2->second.name = album;
				it2->second.time = 0;
				it2->second.nsongs = 0;
				it2->second.time += secs;
				it2->second.songs[track] = Song();
				it2->second.nsongs++;
				for (it3 = it2->second.songs.begin(); it3 != it2->second.songs.end(); ++it3) {
					if (it3->first == track) {
						break;
					}
				}
				it3->second.title = titl;
				it3->second.time = secs;
			}
			/* the code above is for if the artist has been seen before. If the artist has been seen there then has to be a check 
			 to see if the album is new or not. if the album is new we store the album normally. if the album isnt new then we 
			 we find where that album is located in the map to store the song */
			else {
				for (it2 = it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {
                    if (it2->first == album) {
                        break;
                    }
                }
				it2->second.time += secs;
				it2->second.songs[track] = Song();
				it2->second.nsongs++;
				for (it3 = it2->second.songs.begin(); it3 != it2->second.songs.end(); ++it3) {
					if (it3->first == track) {
						break;
					}
				}
				it3->second.title = titl;
				it3->second.time = secs;
			}

		}
		it->second.name = artist;
		it->second.time += secs;
		it->second.nsongs++; 	
	}

	for (it = artists.begin(); it != artists.end(); ++it) {
		cout << it->first << ": " << it->second.nsongs << ", " << convertback(it->second.time) << endl;
		for (it2 = it->second.albums.begin(); it2 != it->second.albums.end(); ++it2) {
			cout << "        " << it2->first << ": " << it2->second.nsongs << ", " << convertback(it2->second.time) << endl;
			for (it3 = it2->second.songs.begin(); it3 != it2->second.songs.end(); ++it3) {
				cout << "                " << it3->first << ". " << it3->second.title << ": " << convertback(it3->second.time) << endl;
			}
		}
	}

	fin.close();

}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
