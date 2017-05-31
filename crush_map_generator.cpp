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

string add_root(float weights[], int &num, string curr_bucket_type){
  string temp = string("root default {") + "\n" + "\talg straw" + "\n" + "\thash 0"+"\n";
  for(int i = 0; i < num; i++){
    temp += string("\titem ") + curr_bucket_type + to_string(i)+ " weight " + to_string(weights[i])+"\n";
  }
  temp += string("}\n") +"\n";
  return temp;
}

string add_buckets(float weights[], int &num, string curr_bucket_type, string next_bucket_type){
  int num_next;  // num_next represents the container buckets for the current set of buckets. 
  string temp;
  if(num/3 > 0){
    num_next = rand()%(num/3) + 1;
  }else{
    num_next = rand()%((num+3)/3) + 1;
  }
  int n = num - (2*num_next);
  int ctr = 0;
  float ar_next[num_next];
  memset(ar_next, 0.0, sizeof(ar_next));
  if(float_rand() < overlap_prob){
    for(int i = 0; i < num_next; i++){
      temp += next_bucket_type + to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      for(int j =(n/3 > 0 ? ((i == num_next-1)? 2+n:2 + rand()%(n/3)): 2+n) ; j > 0; j--){
	ar_next[i] += weights[ctr];
	temp += string("\titem ")+ curr_bucket_type + to_string(ctr) + " weight " + to_string(weights[ctr]) + "\n";
	ctr++;
	n--;
      }
      n+=2;
      temp += string("}")+"\n";
    }
  }else{
    bool check[num];
    vector<int>v;
    memset(check, false, sizeof(check));
    for(int i = 0; i < num_next-1; i++){
      int num_d = rand()%num_next + 1;
      temp += next_bucket_type + to_string(i)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
      v.clear();
      for(int j = num_d; j > 0 ; j--){
	n = rand()%num;
	while(find(v.begin(), v.end(), n) != v.end()){
	  n = rand()%num;
	}
	v.push_back(n);
        temp += string("\titem ")+ curr_bucket_type + to_string(n) + " weight " + to_string(weights[n]) + "\n";
	ar_next[i] += weights[n];
	check[n] = true;
      }
      temp += string("}")+"\n";
    }
    temp += next_bucket_type + to_string(num_next-1)+" {"+"\n"+"\talg straw"+"\n"+"\thash 0"+"\n";
    temp += string("\titem ")+ curr_bucket_type + "0 weight " + to_string(weights[0]) + "\n";
    ar_next[num_next-1] += weights[0];
    for(int i = 1; i < num; i++){
      if(check[i] == false){
	temp += string("\titem ")+ curr_bucket_type + to_string(i)+" weight " + to_string(weights[i]) + "\n";
	ar_next[num_next-1] += weights[i];
      }
    }
    temp += string("}")+"\n";
  }
  temp += string("\n");
  
  // Update the values
  num = num_next;
  for(int i = 0; i < num; i++)
    weights[i] = ar_next[i];
  
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

  float weights[num_osd];
  for(int i = 0; i < num_osd; i++)
    weights[i] = float_rand()*10;

  int n = num_osd;
  s += string("# buckets")+ "\n";
  s += add_buckets(weights, n, string("osd."), string("host host-"));
  s += add_buckets(weights, n, string("host-"), string("datacenter dc-"));
  s += add_root(weights, n, string("dc-"));
  s += add_rules();
  s += finish();
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
