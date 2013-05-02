/*
	This file is part of nationstates-linux.

    nationstates-linux is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nationstates-linux is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nationstates-linux.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <vector>
#include <deque>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <curl/curl.h>
#include <libxml++/libxml++.h>
#include "functions.h"
#include "gtest.h"

using namespace std;

// Converts a Glib::ustring to a char*
const char * functions::strchar(Glib::ustring String){
	const char * Char = String.c_str();
	return Char;
}

string functions::lowercase(string stringer){
	stringer[0] = toupper(stringer[0]);
	for(int i=1; i<stringer.length(); ++i)
		stringer[i] = tolower(stringer[i]);
	return stringer;
}

// Converts a Glib::ustring to a double
double functions::strouble(Glib::ustring String){
	stringstream data(String);
	double Double;
	data >> Double;
	return Double;
}

// I am using this because it output as 0.53 instead of 0.5300000
Glib::ustring functions::doubstr(double number){
	ostringstream ss;
	ss << number;
	return ss.str();
}

// Counts the number of lines in a file (1, 2, 3, ...)
int functions::count_lines(Glib::ustring file){
	ifstream read;
	read.open(strchar(file));
	int nlines = -1;
	string nline;
	while (read.good()){
		getline (read,nline);
		nlines++;
	}
	read.close();
	return nlines;
}

// Function used by curl to write data
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
		size_t written = fwrite(ptr, size, nmemb, stream);
		return written;
}

// Trim a Glib::ustring (string, # of chars to remove from begining, # of chars to remove from end)
Glib::ustring functions::trim(Glib::ustring the_string, int from_start, int from_end){
	return the_string.substr(from_start, the_string.size() - from_start - from_end);
}

// Write all the lines of a file to a ustring vector that is returned
std::vector<Glib::ustring> functions::read(Glib::ustring file){
	ifstream read;
	string filer = file;
	std::vector<Glib::ustring> t_vector;
	read.open(strchar(filer));
	while(getline(read,filer))
		t_vector.push_back(filer);
	read.close();
	return t_vector;
}

Glib::ustring functions::read_single_line(const char * file, int line){
	ifstream read;
	read.open(file);
	string filer;
	for(int i=0; i<line+1; i++)
		getline(read,filer);
	read.close();
	return filer;
}

// Download a file from the internet with (save the file as this name, the link to the file)
void functions::curl_grab(Glib::ustring filed, Glib::ustring url){
	CURL *curl = curl_easy_init();
	if (curl) {
		FILE *fp = fopen(strchar(filed),"wb");
		curl_easy_setopt(curl, CURLOPT_URL, strchar(url));
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(fp);
	}
}

vector<Glib::ustring> functions::convert_times(vector<Glib::ustring> times, Glib::ustring format){
	for(int i=0; i<times.size(); i++){
		time_t thetime =  static_cast<time_t>(std::stod(times.at(i), 0));
		struct tm * timeinfo = localtime(&thetime);
		char buffer [80];
		strftime(buffer, 80, format.c_str(), timeinfo);
		times.at(i) = buffer;
	}
	return times;
}

// Takes the nation name and forms a file called nation.xml that contains all the nation data, also returns the number of lines in the file
int functions::get_nation_data(Glib::ustring nation){
	Glib::ustring nation_url = "http://www.nationstates.net/cgi-bin/api.cgi?nation="+nation+"&q=";
	std::vector<Glib::ustring> url_requests = read("./name-store/url_requests.txt");
	for(int i=0; i<url_requests.size(); i++)
		nation_url = nation_url+"+"+url_requests.at(i);
	curl_grab("./nation.xml", nation_url);
	int size = count_lines("./nation.xml");
	save_census_median();
	return size;
}

// Gets the current time. Mode zero gives the date in ISO YYYY-MM-DD, mode one gives whether it is the morning nationstates or the afternoon, mode one give the current hour HH
Glib::ustring functions::get_time(int mode, bool gmt){
	if(gmt)
		curl_grab("./time.txt", "http://www.timeapi.org/gmt/now?format=%25FT%25R");
	else
		curl_grab("./time.txt", "http://www.timeapi.org/est/now?format=%25FT%25R");
	ifstream read;
	string isotime;
	read.open("./time.txt");
	getline (read,isotime);
	read.close();
	if(mode == 0)
		return trim(isotime, 0, 6);
	else if(mode == 1){
		isotime = trim(isotime, 11, 3);
		if(std::stoi(isotime, 0, 10)<13) isotime = "morning";
		else isotime = "evening";
		return isotime;
	}
	else{
		int iso_time = 24 - std::stoi(trim(isotime, 11, 3), 0, 10);
		if(iso_time>12)
			iso_time = iso_time - 12;
		return std::to_string(iso_time);
	}
}

bool functions::check_for_new_data(vector<Glib::ustring> comparor, std::vector<Glib::ustring> comparee){
	bool return_value = false;
	for(int i=0; i<comparor.size(); i++){
		if(comparor.at(i) != comparee.at(i)){
			return_value = true;
			break;
		}
	}
	return return_value;
}

vector< vector<Glib::ustring> > functions::convert_data(vector<Glib::ustring> all_data, Glib::ustring nation){
	vector< vector<Glib::ustring> > data_vectors;
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

vector<Glib::ustring> functions::load_data(Glib::ustring nation, Glib::ustring save, int bas_cen_man_de_evt_ev_de_bu_ec_fr){
	string line = read_single_line(strchar("./nations-store/"+nation+"/"+save), bas_cen_man_de_evt_ev_de_bu_ec_fr);
	vector<Glib::ustring> new_vector;
	while(line.find(";")!=-1){
		new_vector.push_back(line.substr(0, line.find(";")));
		line = trim(line, line.find(";")+2, 0);
	}
	new_vector.push_back(line);
	return new_vector;
}

// Try to save data if it finds a change value that is != 0 and then add one to the current time if it is before 5, else, use the current time.
void functions::save_data(Glib::ustring current_time, Glib::ustring nation){

	xmlpp::DomParser parser;
	parser.parse_file("./nation.xml");

	vector<Glib::ustring> to_data;
	to_data = print_node(parser.get_document()->get_root_node(), to_data);

	vector< vector<Glib::ustring> > all_data = convert_data(to_data, nation);

	if(access("./name-store/nation_list.txt", F_OK) == -1)
		mkdir("nations-store", S_IRWXU);

	vector<Glib::ustring> previous_dates = read("./nations-store/"+nation+"/datelog.txt");
	bool newdata = false;

	if(previous_dates.size()>0)
		newdata = check_for_new_data(all_data.at(1), load_data(nation, previous_dates.back(), 1));
	//If it does not match or there are no previous dates, it writes a new file
	if((newdata)||(previous_dates.size() == 0)){

		// Checks to see if /nation/datelog exists, if not it makes the directory
		if(access(strchar("./nations-store/"+nation+"/datelog.txt"), F_OK) == -1)
			mkdir(strchar("./nations-store/"+nation), S_IRWXU);

		ofstream save;
		fstream savedate;
		// If the current time is the same as the latest saved and there is new data, save with gmt mode which will force a save with the next filename
		if(previous_dates.size()>0){
			if(current_time == previous_dates.back())
				current_time = get_time(0, true)+"-"+get_time(1, true)+".csv";
		}

		save.open(strchar("./nations-store/"+nation+"/"+current_time));

		for(int l=0; l<all_data.size(); l++){
			for(int i=0; i<all_data.at(l).size()-1; i++)
				save<<all_data.at(l).at(i)<<"; ";
			save<<all_data.at(l).back()<<"\n";
		}

		//Opens the nations datelog file and adds current_date to a new line (a log of data points)
		savedate.open(strchar("./nations-store/"+nation+"/datelog.txt"), fstream::in | fstream::out | fstream::app);
		savedate<<current_time<<"\n";
		save.close();
		savedate.close();
	}

	// Makes a vector of all the nations that have data sets
	std::vector<Glib::ustring> nation_list = read("./name-store/nation_list.txt");
	int first_run = 0; int exists = 0;

	// If there are no data sets
	if(nation_list.size()==0)
		first_run = 1;
	else{
		// For each nation in the log, if it matches, set as 1
		for(int i=0; i<nation_list.size(); i++){
			if(nation == nation_list.at(i))
				exists = 1;
		}
	}
	// If none of the above conditions hold true, save the nation as a new line in the nation_list log
	if((exists == 0)||(first_run == 1)){
		fstream savenation;
		savenation.open(strchar("./name-store/nation_list.txt"), fstream::in | fstream::out | fstream::app);
		savenation<<nation<<"\n";
		savenation.close();
	}

	curl_grab("./nations-store/"+nation+"/flag.jpg", strchar(all_data.at(0).at(4)));
}

// This parses the nation.xml and forms a vector of all the values and titles. Best to pretend this does not exist
std::vector<Glib::ustring> functions::print_node(const xmlpp::Node* node, std::vector<Glib::ustring> new_data_first){

	Glib::ustring finalname;
	const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
	const Glib::ustring nodename = node->get_name();
	if(!nodeText && !nodename.empty())
		finalname = nodename;
	if(nodeText)
		finalname = nodeText->get_content();
	else if(nodeContent)
		finalname = nodeContent->get_content();
	else if(const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(node)){
		const xmlpp::Element::AttributeList& attributes = nodeElement->get_attributes();
		for(xmlpp::Element::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter){
			const xmlpp::Attribute* attribute = *iter;
			finalname = finalname+"-"+attribute->get_value();
		}
	}

	if(finalname.find("\n")==-1)
		new_data_first.push_back(finalname);

	if(!nodeContent){
		xmlpp::Node::NodeList list = node->get_children();
		for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
			new_data_first = print_node(*iter, new_data_first);
	}
	return new_data_first;
}

void functions::save_census_median(){
	curl_grab("./census_medians_temp.txt", "www.nationstates.net/cgi-bin/api.cgi?q=censusmedian");
	vector<Glib::ustring> new_vector = read("./census_medians_temp.txt");
	vector<Glib::ustring> census = read("./nations-store/census_medians.txt");
	Glib::ustring new_value = new_vector.at(0).substr(21, 21);
	if(census.size()<10)
		census.resize(69);
	int first = new_value.find("\"");
	int census_id = std::stoi(new_value.substr(first+1, new_value.find("\"", first+1) - first - 1));
	Glib::ustring census_value = new_value.substr(new_value.find(">")+1, new_value.find("<") - new_value.find(">") - 1);
	census.at(census_id) = census_value;
	ofstream save;
	save.open("./nations-store/census_medians.txt");
	for(int i=0; i<69; i++)
		save<<census.at(i)<<"\n";
	save.close();
}

