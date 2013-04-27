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
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <cstdlib>
#include <curl/curl.h>
#include <libxml++/libxml++.h>
#include "functions.h"
#include "gtest.h"

using namespace std;

// Converts a string to a char*
const char * functions::strchar(string String){
	const char * Char = String.c_str();
	return Char;
}

string functions::lowercase(string string){
	string[0] = toupper(string[0]);
	for(int i=1; i<string.length(); ++i)
		string[i] = tolower(string[i]);
	return string;
}

// Converts a string to a double
double functions::strouble(string String){
	stringstream data(String);
	double Double;
	data >> Double;
	return Double;
}

// I am using this because it output as 0.53 instead of 0.5300000
string functions::doubstr(double number){
	ostringstream ss;
	ss << number;
	return ss.str();
}

// Counts the number of lines in a file (1, 2, 3, ...)
int functions::count_lines(string file){
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

// Trim a string (string, # of chars to remove from begining, # of chars to remove from end)
Glib::ustring functions::trim(Glib::ustring the_string, int from_start, int from_end){
	return the_string.substr(from_start, the_string.size() - from_start - from_end);
}

// Just placing some string formation out here to clean up the gtest class
Glib::ustring functions::make_fullname_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors){
	return "\""+all_data.at(4)+"\"\nCategory: "+all_data.at(6)+"\n"+data_vectors.at(0).at(3)+" million citizens";
}
Glib::ustring functions::make_rights_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors){
	return "Civil Rights: "+data_vectors.at(6).at(0)+" ("+data_vectors.at(0).at(0)+")\nEconomy: "+data_vectors.at(6).at(1)+" ("+
		data_vectors.at(0).at(1)+")\nPolitical Freedom: "+data_vectors.at(6).at(2)+" ("+data_vectors.at(0).at(2)+")\nRegional Influence: "+all_data.at(66)+
		" ("+data_vectors.at(0).at(65)+")";
}
Glib::ustring functions::make_events_text(std::vector< std::vector<Glib::ustring> > data_vectors){
	Glib::ustring events_text;
	for(int i=0; i<data_vectors.at(2).size(); i++){
		events_text = events_text+"<b>"+data_vectors.at(2).at(i)+"</b>\n"+data_vectors.at(2).at(i+1)+"\n\n"; i++;}
	return events_text;
}
Glib::ustring functions::make_description_text(std::vector<Glib::ustring> all_data, std::vector< std::vector<Glib::ustring> > data_vectors, Glib::ustring nation){
	string stringer = data_vectors.at(3).at(6);
	stringer.at(0) = toupper(stringer.at(0));
	Glib::ustring description = all_data.at(2)+" is a "+data_vectors.at(3).at(0)+" nation, renowned for its "+data_vectors.at(3).at(1)+", and "+
		data_vectors.at(3).at(2)+" citizens.\n\n"+data_vectors.at(3).at(4)+" "+data_vectors.at(3).at(5)+"\n\n"+stringer+", "+data_vectors.at(3).at(7)+
		", "+data_vectors.at(3).at(8)+", and "+data_vectors.at(3).at(9)+". "+data_vectors.at(3).at(10)+" "+nation+"\'s national animal is the "+
		data_vectors.at(3).at(11)+", which "+data_vectors.at(3).at(12)+", and its currency is the "+data_vectors.at(3).at(13)+".";
	return description;
}

// Write all the lines of a file to a ustring vector that is returned
std::vector<Glib::ustring> functions::read(string file){
	ifstream read;
	std::vector<Glib::ustring> t_vector;
	read.open(strchar(file));
	while(getline(read,file)){
		t_vector.push_back(file);
	}
	read.close();
	return t_vector;
}

std::string functions::read_single(const char * file, Glib::ustring term){
	ifstream read;
	read.open(file);
	string filer;
	while(getline(read,filer)){
		if(filer == term){
			getline(read,filer);
			break;
		}
	}
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

// Takes the nation name and forms a file called nation.xml that contains all the nation data, also returns the number of lines in the file
int functions::get_nation_data(Glib::ustring nation){
	Glib::ustring nation_url = "http://www.nationstates.net/cgi-bin/api.cgi?nation="+nation+"&q=";
	std::vector<Glib::ustring> url_requests = read("./name-store/url_requests.txt");
	for(int i=0; i<url_requests.size(); i++)
		nation_url = nation_url+"+"+url_requests.at(i);
	curl_grab("./nation.xml", nation_url);
	int size = count_lines("./nation.xml");
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

bool functions::check_for_new_data(std::vector<std::vector<Glib::ustring> > comparor, std::vector<std::vector<Glib::ustring> > comparee){
	bool return_value = false;
	for(int i=0; i<comparor.at(0).size(); i++){
		if(comparor.at(0).at(i) != comparee.at(0).at(i)){
			return_value = true;
		}
	}
	return return_value;
}

// Try to save data if it finds a change value that is != 0 and then add one to the current time if it is before 5, else, use the current time.
void functions::save_data(std::vector<Glib::ustring> all_data, Glib::ustring current_time, Glib::ustring nation){

	if(access("./name-store/nation_list.txt", F_OK) == -1)
		mkdir("nations-store", S_IRWXU);

	std::vector<Glib::ustring> previous_dates = read("./nations-store/"+nation+"/datelog.txt");
	bool newdata = false;
	if(previous_dates.size()>0){
		std::vector<std::vector<Glib::ustring> > comparor = last_vectors_generate(all_data);
		std::vector<std::vector<Glib::ustring> > comparee = last_vectors_generate(read("./nations-store/"+nation+"/"+previous_dates.back()));
		newdata = check_for_new_data(comparor, comparee);
	}
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
				current_time = get_time(0, true)+"-"+get_time(1, true)+".txt";
		}

		save.open(strchar("./nations-store/"+nation+"/"+current_time));
		for(int i=0; i<all_data.size(); i++)
			save<<all_data.at(i)<<"\n";

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

	if((newdata)||(previous_dates.size() == 0)){
		gTest::instance().force_notebook_refresh(0);
		gTest::instance().refresh_saves();
	}
}

// Sets the error message for a popup window and shows it.
void functions::set_error(Gtk::Window* errorPopup, Glib::ustring title, Glib::ustring error_message){
	errorPopup->set_title(title);
	errorPopup->add_label(error_message, false, 0.5, 0.5);
	errorPopup->show_all();
}

// Sets up the error popup window.
Gtk::Window* functions::error_setup(){
	Gtk::Window* errorPopup;
	errorPopup = Gtk::manage (new Gtk::Window());
	errorPopup->set_position (Gtk::WIN_POS_CENTER_ON_PARENT);
	errorPopup->set_modal(true);
	errorPopup->set_border_width(10);;
	return errorPopup;
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

// Takes the data vector formed by print_node and searches for certain elements that we want. Pushes them to new vectors to later print.
std::vector<std::vector<Glib::ustring> > functions::vectors_generate(std::vector<Glib::ustring> all_data, Glib::ustring nation){

	std::vector<std::vector<Glib::ustring> > data_vectors;
	std::vector<Glib::ustring> census;
	std::vector<Glib::ustring> deaths;
	std::vector<Glib::ustring> events;
	std::vector<Glib::ustring> descriptions (14, "");
	std::vector<Glib::ustring> budget;
	std::vector<Glib::ustring> economy;
	std::vector<Glib::ustring> freedoms;

	for(int i=0; i<all_data.size(); i++){
		if(all_data.at(i).find("CENSUS")!=-1){
			census.push_back(all_data.at(i+1)); i++;}
		else if(all_data.at(i).find("CAUSE")!=-1){
			deaths.push_back(all_data.at(i).erase(0,6));
			deaths.push_back(trim(all_data.at(i+1), 0, 1)); i++;}
		else if(all_data.at(i).find("EVENT")!=-1){
			time_t thetime =  static_cast<time_t>(std::stod(all_data.at(i+2), 0));
			struct tm * timeinfo = localtime(&thetime);
			char buffer [80];
			strftime(buffer, 80, "%d %B, %Y, %R", timeinfo);
			events.push_back(buffer);
			while(all_data.at(i+4).find("@@")!=-1)
				all_data.at(i+4) = all_data.at(i+4).replace(all_data.at(i+4).find("@@"), nation.size() +4, nation);
			events.push_back(all_data.at(i+4)); i = i+4;}
		else if(all_data.at(i) == "ADMIRABLE"){
			descriptions.at(0) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "NOTABLE"){
			descriptions.at(1) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "SENSIBILITIES"){
			descriptions.at(2) = all_data.at(i+1); i++;}
		else if(all_data.at(i) == "POPULATION"){
			descriptions.at(3) = all_data.at(i+1); i++;}
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
	data_vectors.push_back(census);
	data_vectors.push_back(deaths);
	data_vectors.push_back(events);
	data_vectors.push_back(descriptions);
	data_vectors.push_back(budget);
	data_vectors.push_back(economy);
	data_vectors.push_back(freedoms);
	return data_vectors;
}

// Does the same as generate vectors but without some things.
std::vector<std::vector<Glib::ustring> > functions::last_vectors_generate(std::vector<Glib::ustring> last_data){

	std::vector<std::vector<Glib::ustring> > last_vectors;
	std::vector<Glib::ustring> last_census;
	std::vector<Glib::ustring> last_deaths;
	std::vector<Glib::ustring> last_budget;
	std::vector<Glib::ustring> last_economy;
	for(int i=0; i<last_data.size(); i++){
		if(last_data.at(i).find("CENSUS")!=-1){
			last_census.push_back(last_data.at(i+1)); i++;}
		else if(last_data.at(i).find("CAUSE")!=-1){
			last_deaths.push_back(last_data.at(i).erase(0,6));
			last_deaths.push_back(trim(last_data.at(i+1), 0, 1)); i++;}
		else if(last_data.at(i) == "TAX"){
			last_economy.push_back(last_data.at(i+1)); i++;}
		else if(last_data.at(i) == "PUBLICSECTOR"){
			last_economy.push_back(trim(last_data.at(i+1), 0, 1));
			last_economy.push_back(std::to_string(100-std::stoi(last_data.at(i+1), 0, 10))); i++;}
		else if(last_data.at(i) == "ENVIRONMENT"){
			for(int j=0; j<11; j++)
				last_budget.push_back(last_data.at(i+1+(j*2)));
			i=i+21;
		}
	}
	last_vectors.push_back(last_census);
	last_vectors.push_back(last_deaths);
	last_vectors.push_back(last_economy);
	last_vectors.push_back(last_budget);
	return last_vectors;
}

std::vector<Glib::ustring> functions::get_deaths(const char * latest_deaths, Glib::ustring nation){
	ifstream read_death;
	read_death.open("./nations-store/"+nation+"/"+latest_deaths);
	string dBuffer;
	std::vector<Glib::ustring> all_deaths;
	while(getline(read_death, dBuffer)){
		if(dBuffer.find("CAUSE-")!=-1){
			while(dBuffer.find("CAUSE-")!=-1){
				for(int i=0; i<2; i++){
					all_deaths.push_back(dBuffer);
					getline(read_death, dBuffer);
				}
			}
		break;
		}
	}
	read_death.close();
	return all_deaths;
}

