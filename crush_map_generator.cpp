#include <bits/stdc++.h>
using namespace std;

static const float overlap_prob = 0.8;
int num_of_crushmaps;

float float_rand(){
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

string init(){
  string temp;
  temp = std::string("# begin crush map")+"\n"+
    "tunable choose_local_tries 0"+"\n"+
    "tunable choose_local_fallback_tries 0"+"\n"+
    "tunable choose_total_tries 50"+"\n"+
    "tunable chooseleaf_descend_once 0"+"\n"
    "tunable chooseleaf_vary_r 0"+"\n"+"\n";
  return temp;
}

void write_to_file(string s){
  string temp = "./crushmaps/crushmap-" + to_string(num_of_crushmaps) + ".txt";
  ofstream temp_file(temp);
  temp_file<<s;
  temp_file.close();
}

string add_devices(int num){
  string temp = string("# devices")+"\n";
  for(int i = 0; i < num; i++){
    temp += string("device ") + to_string(i)+ " osd." + to_string(i)+"\n";
  }
  temp += "\n";
  return temp;
}

string set_types(){
  string temp = string("# types") + "\n"+
    "type 0 osd" + "\n"+
    "type 1 host" + "\n"+
    "type 2 chassis" + "\n" +
    "type 3 rack" + "\n" +
    "type 4 row" + "\n" +
    "type 5 room" + "\n" +
    "type 6 datacenter" + "\n"+
    "type 7 region" + "\n"+
    "type 8 root"+"\n"+"\n";
  return temp;
}

string add_root(int num, float ar_dc[]){
  string temp = string("root default {") + "\n" + "\talg straw" + "\n" + "\thash 0"+"\n";
  for(int i = 0; i < num; i++){
    temp += string("\titem dc-") + to_string(i)+ " weight " + to_string(ar_dc[i])+"\n";
  }
  temp += string("}\n") +"\n";
  return temp;
}

string add_datacenters(float ar_hosts[], int num){
  string temp;
  int num_dc;
  if(num/3 > 0){
    num_dc = rand()%(num/3) + 1;
  }else{
    num_dc = rand()%((num+3)/3) + 1;
  }
  int n = num - (2*num_dc);
  int ctr = 0;
  float ar_dc[num_dc];
  memset(ar_dc,0.0, sizeof(ar_dc));
  if(float_rand() < overlap_prob){
    for(int i = 0; i < num_dc; i++){
      temp += string("datacenter dc-")+ to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      for(int j =(n/3 > 0 ? ((i == num_dc-1)? 2+n:2 + rand()%(n/3)): 2+n) ; j > 0; j--){
	ar_dc[i] += ar_hosts[ctr];
	temp += string("\titem host-") + to_string(ctr) + " weight " + to_string(ar_hosts[j]) + "\n";
	ctr++;
	n--;
      }
      n+=2;
      temp += string("}")+"\n";
    }
  }else{
    bool check[num];
    memset(check, false, sizeof(check));
    for(int i = 0; i < num_dc-1; i++){
      int num_hosts = rand()%num_dc + 1;
      temp += string("datacenter dc-")+ to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      for(int j = num_hosts; j > 0 ; j--){
	n = rand()%num;
        temp += string("\titem host-") + to_string(n) + " weight " + to_string(ar_hosts[n]) + "\n";
	ar_dc[i] += ar_hosts[n];
	check[n] = true;
      }
      temp += string("}")+"\n";
    }
    temp += string("datacenter dc-")+ to_string(num_dc-1)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
    temp += string("\titem host-0")+ " weight " + to_string(ar_hosts[0]) + "\n";
    ar_dc[num_dc-1] += ar_hosts[0];
    for(int i = 1; i < num; i++){
      if(check[i] == false){
	temp += string("\titem host-")+ to_string(i)+" weight " + to_string(ar_hosts[i]) + "\n";
	ar_dc[num_dc-1] += ar_hosts[i];
      }
    }
    temp += string("}")+"\n\n";
  }
  temp += add_root(num_dc,ar_dc);
  return temp;
}

string add_buckets(int num){
  string temp = string("# buckets")+ "\n";

  float ar_dev_weights[num];
  for(int i = 0; i < num; i++)
    ar_dev_weights[i] = float_rand()*10;
  
  // Adding host buckets.
  int num_hosts;
  if(num/3 > 0){
    num_hosts = rand()%(num/3) + 1;
  }else{
    num_hosts = rand()%((num+3)/3) + 1;
  }
  int n = num - (2*num_hosts);
  int ctr = 0;
  float ar_hosts[num_hosts];
  memset(ar_hosts,0.0, sizeof(ar_hosts));
  if(float_rand() < overlap_prob){
    for(int i = 0; i < num_hosts; i++){
      temp += string("host host-")+ to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      for(int j =(n/3 > 0 ? ((i == num_hosts-1)? 2+n:2 + rand()%(n/3)): 2+n) ; j > 0; j--){
	ar_hosts[i] += ar_dev_weights[ctr];
	temp += string("\titem osd.") + to_string(ctr) + " weight " + to_string(ar_dev_weights[j]) + "\n";
	ctr++;
	n--;
      }
      n+=2;
      temp += string("}")+"\n";
    }
  }else{
    bool check[num];
    memset(check, false, sizeof(check));
    for(int i = 0; i < num_hosts-1; i++){
      int num_devices = rand()%num_hosts + 1;
      temp += string("host host-")+ to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      for(int j = num_devices; j > 0 ; j--){
	n = rand()%num;
        temp += string("\titem osd.") + to_string(n) + " weight " + to_string(ar_dev_weights[n]) + "\n";
	ar_hosts[i] += ar_dev_weights[n];
	check[n] = true;
      }
      temp += string("}")+"\n";
    }
    temp += string("host host-")+ to_string(num_hosts-1)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
    temp += string("\titem osd.0")+ " weight " + to_string(ar_dev_weights[0]) + "\n";
    ar_hosts[num_hosts-1] += ar_dev_weights[0];
    for(int i = 1; i < num; i++){
      if(check[i] == false){
	temp += string("\titem osd")+ to_string(i)+" weight " + to_string(ar_dev_weights[i]) + "\n";
	ar_hosts[num_hosts-1] += ar_dev_weights[i];
      }
    }
    temp += string("}")+"\n";
  }

  // Adding datacenter buckets
  temp += "\n" + add_datacenters(ar_hosts, num_hosts);
  return temp;
}

string add_rules(){
  string temp = string("rule rule-0 {")+"\n"+
    "\truleset 0"+"\n"+
    "\ttype replicated"+"\n"+
    "\tmin_size 0"+"\n"+
    "\tmax_size 10"+"\n"+
    "\tstep take default"+"\n"+
    "\tstep choose firstn 0 type datacenter"+"\n"+
    "\tstep chooseleaf firstn 1 type host"+"\n"+
    "\tstep emit"+"\n"+
    "}"+"\n";
  return temp;
}

string finish(){
  string temp = string("\n") + string("# end crush map");
  return temp;
}

void create_crushmap(){
  string s = init();
  int num_osd = rand()%100 + 1;
  s = s + add_devices(num_osd);
  s = s + set_types();
  s = s + add_buckets(num_osd);
  s = s + add_rules();
  s = s + finish();
  write_to_file(s);
}

int main(){
  srand (time(NULL));
  cout<<"Enter the number of crushmaps you want to add: ";
  cin>>num_of_crushmaps;
  while(num_of_crushmaps--){
    create_crushmap();
  }
  
  return 0;  
}
