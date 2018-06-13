#include <bits/stdc++.h>

using namespace std ;

class Include_Remover
{
public:
	vector<string> v ;
	string cstr , tstr ;
	bool ok , is_valid ;

	Include_Remover() {}

	Include_Remover(vector<string>v)
	{
		this->v = v ;
		cstr = "#include" ;
	}

	void Process()
	{
		int ln , cont ;
		FILE *fin ;
		fin = fopen("t_em_p.cpp","w");

		for(int i=0 ; i<v.size() ; i++)
		{
			tstr = "" ;
			ok = false ;
			is_valid = true ;
			cont = 0 ;

			ln = v[i].size() ;

			if(ln == 0)
				continue ;

			for(int j=0 ; j<ln ; j++)
			{
				if(ok)
				{
					tstr += v[i][j] ;
					cont++;
// cout<<tstr<< " " << tstr.size() << endl ;
					if(cont == 8)
					{
						if(tstr == cstr)
						{
							is_valid = false ;
							break;
						}
					}
				}

				else
				{
					if(v[i][j] != ' ')
					{
						tstr += v[i][j] ;
						ok = true ;
						cont++;
					}
				}
			}

			if(is_valid)
				fprintf(fin, "%s\n", tstr.c_str());
		}
		fclose(fin);
	}
};

class Edit_Distance
{
public:
	string str1 , str2 ;
	int ln1 , ln2 ;
	int **dp ;

	Edit_Distance() {}

	Edit_Distance(string str1,string str2)
	{
		this->str1 = str1 ;
		this->str2 = str2 ;

		ln1 = str1.length() ;
		ln2 = str2.length() ;

		dp = new int*[ln1+7] ;

		for(int i=0 ; i<ln1+7 ; i++)
			dp[i] = new int[ln2+7] ;

		for(int i=0 ; i<=ln2 ; i++)
			dp[0][i] = i ;

		for(int i=0 ; i<=ln1 ; i++)
			dp[i][0] = i ;
	}

	int cal_culate()
	{
		for(int i=1 ; i<=ln1 ; i++)
		{
			for(int j=1 ; j<=ln2 ; j++)
			{
				dp[i][j] = min(dp[i-1][j],dp[i][j-1]) + 1 ;

				if(str1[i-1] == str2[j-1])
					dp[i][j] = min(dp[i][j],dp[i-1][j-1]) ;
			}
		}

		double mismatch = dp[ln1][ln2]/(1.0 * (ln1 + ln2)) ;
		double match = 1.0 - mismatch ;
		match = match*100.0 ;

		return round(match) ;
	}
};

vector<string>inp_vec ;
string inp_str ;

void pre_process_file(string name)
{
	ifstream cin(name); // argv[2] is input file
	inp_vec.clear();

	while(getline(cin,inp_str))
	{
		inp_vec.push_back(inp_str);
		// cout<<inp_str<<endl ;
	}

	Include_Remover *rmvr = new Include_Remover(inp_vec);
	rmvr->Process();

	system("g++ -E t_em_p.cpp -o a_f_t_e_r_process.txt"); // command for replacing all macros , prerequisite: g++ global compiler
}

bool is_valid(char ch)
{
    if(ch >= 'A' && ch <= 'Z')
        return false ;

    if(ch >= 'a' && ch <= 'z')
        return false ;

    if(ch >= '0' && ch <= '9')
        return false ;

    if(ch == '_' || ch == '\n' || ch == ' ' || ch == '\r')
        return false ;

    return true ;
}

string process_a_file(string name)
{
	pre_process_file(name);

	ifstream cin("a_f_t_e_r_process.txt");
	inp_vec.clear();
	int ln ;
	bool ok = false ;
	string tstr = "" ;


	while(getline(cin,inp_str))
	{
		ln = inp_str.size() ;

		if(ln == 0)
			continue;

		ok = true ;

		for(int i=0 ; i<ln ; i++)
		{
			if(inp_str[i] != ' ')
			{
				ok = (inp_str[i] != '#');
				break;
			}
		}

		if(ok)
		{
			for(int i=0 ; i<ln ; i++)
			{
				if(is_valid(inp_str[i]))
					tstr += inp_str[i] ;
			}
		}
	}

	// cout<<"process " << name << " " << tstr << endl ;

	return tstr ;
}

vector< pair<string,string> >all_process_file ;

int main(int argc, char *argv[])
{
	string file_content ;
	file_content = process_a_file(argv[2]);

	string path = argv[2] , submission_id_path , submission_path , submition_id ;

	submission_id_path = "submition_id.txt";
	
	ifstream cin(submission_id_path);
	int ln ;
	bool ok = true ;
// cout << "--> " << submission_id_path << endl ;
	while(getline(cin,inp_str))
	{
		ln = inp_str.size() ;
// cout<< "inner " << ln << " " << inp_str << endl ;
		if(ln == 0)
			continue ;

		submission_path = path + "\\" ;
		submition_id = "" ;
		ok = true ;

		for(int i=0 ; i<ln ; i++)
		{
			if(inp_str[i] == ',')
			{
				submission_path += '\\' ;
				submission_path += submition_id ;
				submission_path += '.' ;
				ok = false ;
			}

			else
				submission_path += inp_str[i] ;

			if(ok)
				submition_id += inp_str[i] ;
		}
 // cout << submition_id << " " << submission_path << endl ;
		all_process_file.push_back(make_pair(submition_id,process_a_file(submission_path.c_str())));
		// cout<<submition_id << " " << all_process_file.back().second<<endl ;
	}

	ofstream out("result.csv");

	ln = all_process_file.size() ;

	for(int i=0 ; i<ln ; i++)
	{
		for(int j=i+1 ; j<ln ; j++)
		{
			Edit_Distance *calculator = new Edit_Distance(all_process_file[i].second,all_process_file[j].second);
			out<<all_process_file[i].first << "," << all_process_file[j].first << "," << calculator->cal_culate() << endl ;
		}
	}

	return 0 ;
}
