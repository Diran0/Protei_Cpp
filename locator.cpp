#include <iostream>
#include <fstream>
#include </usr/include/nlohmann/json.hpp>
#include <thread>
#include <math.h>

bool InZone(float x1, float y1, float x2, float y2, float r){if((x1!=x2)||(y1!=y2)){
if (pow((pow((x1-x2),2) +pow((y1-y2),2)),(0.5)) <r)
{return true;}
else{return false;}}
else{return true;}

}


int main() {

//spdlog::info("hello");
bool Flag_Trigger=false;
bool Flag_Trigger1=false;
bool Flag_Stop=false;
bool Flag1=false;
std::string message="";
std::ifstream zones;
zones.open(R"(config.json)", std::ios::in);
std::fstream abonents;
abonents.open(R"(abonent.json)", std::ios::in);
nlohmann::json Zon;
nlohmann::json Abon;

///////////////////////////////////
std::thread configs([&]()
{
	while(!Flag_Stop){
	std::ifstream zones1;
	zones1.open(R"(config.json)", std::ios::in);
	std::fstream abonents1;
	abonents1.open(R"(abonent.json)", std::ios::in);
	nlohmann::json Doc1{nlohmann::json::parse(abonents1)};
	nlohmann::json Doc2{nlohmann::json::parse(zones1)};
	
	if(Zon != Doc2){
	std::cout<<"Zones: "<<Doc2[0]["zones"]<<std::endl;
	Zon = Doc2;
	}
	if(Abon != Doc1){
	std::cout<<"Abonents: "<<Doc1[0]["abonents"]<<std::endl;
	Abon = Doc1;
	}
	Flag1 = true;
	}
});
/////////////////////////////////////////
std::thread choose([&]()
{
	while(!Flag_Stop){
	std::cout<<"Chose:"<<std::endl;
	std::cout<<" Stop - to break program;"<<std::endl;
	std::cout<<" Check_Abonent - to know abonent's zone;"<<std::endl;
	std::cout<<" Check_Zone - to know, who in zone of abonents;"<<std::endl;
	std::cout<<" Trigger(0 or 1) - to enable/diable triggers;"<<std::endl;
	std::string i="0";
	std::cin>>i;
	if(i=="Stop"){ 
			Flag_Stop=true;
			}
	else if (i =="Check_Abonent"){ 
			std::cout<<"Choose abonent's id: ";
			std::string a_id;
			std::cin>>a_id;
			std::cout<<std::endl;
			for(int j = 0; j < Abon[0]["abonents"].size(); j++)
		{
		if(a_id==Abon[0]["abonents"][j]["id"]){
		std::cout<<a_id<<" in zone:"<<std::endl;
				for(int k = 0; k < Zon[0]["zones"].size(); k++)
		{
		if(InZone(Abon[0]["abonents"][j]["x"],Abon[0]["abonents"][j]["y"],Zon[0]["zones"][k]["x"],Zon[0]["zones"][k]["y"],Zon[0]["zones"][k]["radius"]))
		{std::cout<<" "<< Zon[0]["zones"][k]["name"]<<std::endl;}
		}
		}
		}
			}
	else if (i =="Check_Zone"){ 
			std::cout<<"Choose zone's id: ";
			int a_id;
			std::cin>>a_id;
			std::cout<<std::endl;
			for(int j = 0; j < Zon[0]["zones"].size(); j++)
		{
		if(a_id==Zon[0]["zones"][j]["id"]){
		std::cout<<" In zone "<< Zon[0]["zones"][j]["name"]<<":"<<std::endl;
				for(int k = 0; k < Abon[0]["abonents"].size(); k++)
		{
		if(InZone(Abon[0]["abonents"][j]["x"],Abon[0]["abonents"][j]["y"],Zon[0]["zones"][k]["x"],Zon[0]["zones"][k]["y"],Zon[0]["zones"][k]["radius"]))
		{std::cout<<" Abonent "<< Abon[0]["abonents"][k]["id"]<<std::endl;}
		}
		}
		}
			}
		else if (i =="Trigger0"){Flag_Trigger =!Flag_Trigger;}
		else if (i =="Trigger1"){Flag_Trigger1 =!Flag_Trigger1;}
		else{
			std::cout<<"Retry"<<std::endl;
			}
	}
	
});
/////////////////////////////////////////////////
std::thread triger([&]()
{
	float x1,x2,y1,y2,r;
	std::string message1;
	
	nlohmann::json Test_Json = Abon;
	while(!Flag_Stop){
	if (Test_Json!=Abon){
	std::ifstream trigs;
	trigs.open(R"(triggers.json)", std::ios::in);
	nlohmann::json Doc2{nlohmann::json::parse(trigs)};
	Test_Json = Abon;
	x1,x2,y1,y2,r = 0;
	if(Flag_Trigger){

	
	//abonents triggers
	
	for(int i=0; i<Doc2[0]["abonents"].size();i++){
		for(int j=0; j<Abon[0]["abonents"].size();j++){
		if (Doc2[0]["abonents"][i]["subscriber1_id"] == Abon[0]["abonents"][j]["id"]){
		x1 = Abon[0]["abonents"][j]["x"];
		y1 = Abon[0]["abonents"][j]["y"];}
		if (Doc2[0]["abonents"][i]["subscriber2_id"] == Abon[0]["abonents"][j]["id"]){
		x2 = Abon[0]["abonents"][j]["x"];
		y2 = Abon[0]["abonents"][j]["y"];}}
		if(InZone(x1,y1,x2,y2,Doc2[0]["abonents"][i]["distance"])){
		message1 = "Trigger: abonent";
		message1 +=(Doc2[0]["abonents"][i]["id"]);
		if(message != message1){
		message = message1;
		std::cout<<message1<<std::endl;
		}
		}
	}}
	//zone triggers
	if(Flag_Trigger1){
	
		for(int i=0; i<Doc2[0]["zone"].size();i++){
	
		for(int j=0; j<Abon[0]["abonents"].size();j++){
		if (Doc2[0]["zone"][i]["subscriber_id"] == Abon[0]["abonents"][j]["id"]){
		x1 = Abon[0]["abonents"][j]["x"];
		y1 = Abon[0]["abonents"][j]["y"];}
		}
		for(int j=0; j<Zon[0]["zones"].size();j++){
		if (Doc2[0]["zone"][i]["zone_id"] == Zon[0]["zones"][j]["id"]){
		x2 =Zon[0]["zones"][j]["x"];
		y2 =Zon[0]["zones"][j]["y"];
		r = Zon[0]["zones"][j]["radius"];
		}}
		if(InZone(x1,y1,x2,y2,r) && (Doc2[0]["zone"][i]["event"] != "out" )){
		message1 = "Trigger: zone";
		message1 +=(Doc2[0]["zone"][i]["id"]);
		message1 +=" IN";
		if(message != message1){
		message = message1;
		std::cout<<message1<<std::endl;
		}
		}
		else if(!InZone(x1,y1,x2,y2,r) && (Doc2[0]["zone"][i]["event"] != "in" )){
		message1 = "Trigger: zone";
		message1 +=(Doc2[0]["zone"][i]["id"]);
		message1 +=" OUT";
		if(message != message1){
		message = message1;

		}
		}
	}
	}}}
});
choose.join();
configs.join();
triger.join();
	return 0;
}
