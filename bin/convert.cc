#include <sstream>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <deque>


using namespace std;

const char * strchar(Glib::ustring String){
	const char * Char = String.c_str();
	return Char;
}

vector<Glib::ustring> read(Glib::ustring file){
	ifstream read;
	vector<Glib::ustring> t_vector;
	read.open(strchar(file));
	while(getline(read,file))
		t_vector.push_back(file);
	read.close();
	return t_vector;
}

string trim(Glib::ustring the_string, int from_start, int from_end){
	return the_string.substr(from_start, the_string.size() - from_start - from_end);
}

vector< vector<Glib::ustring> > convert(vector<Glib::ustring> all_data, Glib::ustring nation){

	vector<vector<Glib::ustring> > data_vectors;
	vector<Glib::ustring> census;
	deque<string> manufacturing;
	vector<Glib::ustring> man;
	vector<Glib::ustring> deaths;
	vector<Glib::ustring> events;
	vector<Glib::ustring> events_times;
	vector<Glib::ustring> descriptions (14, "");
	vector<Glib::ustring> budget;
	vector<Glib::ustring> economy;
	vector<Glib::ustring> freedoms;
	vector<Glib::ustring> basics;
	vector<Glib::ustring> man_vector {"E-10", "E-11", "E-12", "E-13", "E-14", "E-15", "E-16", "E-17", "E-18", "E-19", "E-20", "E-21", "E-22", "E-23", "E-24", "E-25"};
	vector<Glib::ustring> basics_vector {"FULLNAME", "MOTTO", "CATEGORY", "REGION", "FLAG"};
	bool found = false;

	for(int i=0; i<all_data.size(); i++){
		found = false;
		for(int j=0; j<basics_vector.size(); j++){
			if(all_data.at(i).find(basics_vector.at(j))!=-1){
				basics.push_back(all_data.at(i+1));
				i++;
				break;
			}
		}
		if(all_data.at(i).find("INFLUENCE")!=-1){
			basics.push_back(all_data.at(i+1));
			i++;
		}
		else if(all_data.at(i).find("CENSUS")!=-1){
			for(int j=0; j<man_vector.size(); j++){
				if(all_data.at(i).find(man_vector.at(j))!=-1){
					manufacturing.push_back(all_data.at(i+1));
					found = true;
					break;
				}
			}
			if(all_data.at(i).find("E-26")!=-1)
				manufacturing.push_front(all_data.at(i+1));
			else if((all_data.at(i).find("E-3")!=-1)&&(found==false)){
				descriptions.at(3) = all_data.at(i+1);
				census.push_back(all_data.at(i+1));
				found = true;
			}
			else if(found == false)
				census.push_back(all_data.at(i+1));
			i++;
		}
		else if(all_data.at(i).find("CAUSE")!=-1){
			deaths.push_back(all_data.at(i).erase(0,6));
			deaths.push_back(trim(all_data.at(i+1), 0, 1));
			i++;
		}
		else if(all_data.at(i).find("EVENT")!=-1){
			events_times.push_back(all_data.at(i+2));
			while(all_data.at(i+4).find("@@")!=-1)
				all_data.at(i+4) = all_data.at(i+4).replace(all_data.at(i+4).find("@@"), nation.size() +4, nation);
			events.push_back(all_data.at(i+4));
			i = i+4;
		}
		else if(all_data.at(i) == "ADMIRABLE"){
			descriptions.at(0) = all_data.at(i+1);
			i++;
		}
		else if(all_data.at(i) == "NOTABLE"){
			descriptions.at(1) = all_data.at(i+1);
			i++;
		}
		else if(all_data.at(i) == "SENSIBILITIES"){
			descriptions.at(2) = all_data.at(i+1);
			i++;
		}
		else if(all_data.at(i) == "GOVTDESC"){
			descriptions.at(4) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "INDUSTRYDESC"){
			descriptions.at(5) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "LAW"){
			descriptions.at(6) = all_data.at(i+1); descriptions.at(7) = all_data.at(i+3);
			descriptions.at(8) = all_data.at(i+5); descriptions.at(9) = all_data.at(i+7); i=i+7;}
		else if(all_data.at(i) == "CRIME"){
			descriptions.at(10) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "ANIMAL"){
			descriptions.at(11) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "ANIMALTRAIT"){
			descriptions.at(12) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "CURRENCY"){
			descriptions.at(13) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "ENVIRONMENT"){
			for(int j=0; j<11; j++)
				budget.push_back(all_data.at(i+1+(j*2)));
			i=i+21;
		}
		else if(all_data.at(i) == "TAX"){
			economy.push_back(all_data.at(i+1)); i++;}
		else if(all_data.at(i) == "PUBLICSECTOR"){
			economy.push_back(trim(all_data.at(i+1), 0, 1));
			economy.push_back(std::to_string(100-std::stoi(all_data.at(i+1), 0, 10))); i++;}
		else if(all_data.at(i) == "FREEDOM"){
			for(int j=0; j<3; j++)
				freedoms.push_back(all_data.at(i+2+(j*2)));
		i=i+6;
		}

	}

	for(int i=0; i<manufacturing.size(); i++)
		man.push_back(manufacturing.at(i));

	data_vectors.push_back(basics);
	data_vectors.push_back(census);
	data_vectors.push_back(man);
	data_vectors.push_back(deaths);
	data_vectors.push_back(events_times);
	data_vectors.push_back(events);
	data_vectors.push_back(descriptions);
	data_vectors.push_back(budget);
	data_vectors.push_back(economy);
	data_vectors.push_back(freedoms);
	return data_vectors;
}

int main(){

	vector<Glib::ustring> nation_list = read("./name-store/nation_list.txt");
	vector<Glib::ustring> previous_dates;

	for(int j=0; j<nation_list.size();j++){

		previous_dates.clear();
		previous_dates = read("./nations-store/"+nation_list.at(j)+"/datelog.txt");

		for(int k=0; k<previous_dates.size(); k++){
			vector<Glib::ustring> old_data = read(strchar("./nations-store/"+nation_list.at(j)+"/"+previous_dates.at(k)));

			if(old_data.size()>10){
				vector<vector<Glib::ustring> > new_data = convert(old_data, nation_list.at(j));

				ofstream save;
				save.open(strchar("./nations-store/"+nation_list.at(j)+"/"+previous_dates.at(k)));
				for(int l=0; l<new_data.size(); l++){
					for(int i=0; i<new_data.at(l).size()-1; i++)
						save<<new_data.at(l).at(i)<<"; ";
					save<<new_data.at(l).back()<<"\n";
				}
				save.close();
			}
			if(previous_dates.at(k).find(".txt")!=-1){
				previous_dates.at(k) = trim(previous_dates.at(k),0,4);
				int result = rename(strchar("./nations-store/"+nation_list.at(j)+"/"+previous_dates.at(k)+".txt"), strchar("./nations-store/"+nation_list.at(j)+"/"+previous_dates.at(k)+".csv"));
				ofstream savedate;
				savedate.open(strchar("./nations-store/"+nation_list.at(j)+"/datelog.txt"));
				for(int i=0; i<previous_dates.size(); i++)
					savedate<<previous_dates.at(i)<<".csv\n";
				savedate.close();
			}
			else if(previous_dates.at(k).find(".csv")==-1){
				ofstream savedate;
				savedate.open(strchar("./nations-store/"+nation_list.at(j)+"/datelog.txt"));
				for(int i=0; i<previous_dates.size(); i++)
					savedate<<previous_dates.at(i)<<".csv\n";
				savedate.close();
			}
		}
	}
	return 0;
}
