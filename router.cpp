#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include<fstream>
//#include <bits/stdc.h>
#include<bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<vector>
#include<map>
#include <unistd.h>
#include<string>
#define inf 1e9

using namespace std;
struct router{
    char ip[15];
    char nxt[15];
    int cost;
};
char thisip[100];
string strthisip;
int sockfd,sockfd1;
socklen_t addrlen;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
int f=0,k=0;
string str;
map < string,int > m;
map < string,int > dd;
map <string,int> cnt;
map <string, map <string,int> > temp1;
vector<string> nghbr,v,routerrecv,nghbrcost;
char ip[15];
struct router rtr[5],rtp[5],temp[5];
int strcmp1(char *s,char *p)
{
    int i;
    for(i=0;i<12;i++){
        if(s[i]=='\0')break;
        if(p[i]=='\0')break;
        if(s[i]!=p[i]){return 1;}
    }
    return 0;
}
int copy(struct router rtr[5],struct router ab[5])
{
    for(int i=0;i<k;i++){
	strcpy(ab[i].ip,rtr[i].ip);
	strcpy(ab[i].nxt,rtr[i].nxt);
	ab[i].cost=rtr[i].cost;
    }

}
int strcmp2(char *s,char *p)
{
    int i;
    for(i=0;i<11;i++){
        if(s[i]=='\0')break;
        if(p[i]=='\0')break;
        if(s[i]!=p[i]){return 1;}
    }
    return 0;
}
void init(char ip[]){
    FILE *fptr;
    char s[15];
    char dest[15];
    char cost[15];
    int j=0;
    fptr = fopen("topo.txt", "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    char ch;
    while (ch != EOF)
    {
        ch = fgetc(fptr);
        if(ch!='\n')s[j++]=ch;
        if(ch==' '){
            s[j]='\0';
            j=0;
            if(strcmp1(s,ip)==0){
                f=1;
		str=s;
                if(m[str]==0){
                    v.push_back(str);
                    m[str]=1;
                }
            }
            else if(strcmp2(s,ip)==0){
                strcpy(dest,s);
                str=s;
                if(m[str]==0){
                    v.push_back(str);
                    m[str]=1;
                }
            }
            memset(s,'\0',15);
        }
        else if(ch=='\n'){
            s[j]='\0';
            j=0;
            strcpy(cost,s);
            //printf("%s %s %s\n",dest,dest,cost);
            if(f==1){
                strcpy(rtr[k].ip,dest);
		dd[rtr[k].ip]=-1;
                strcpy(rtr[k].nxt,dest);
                str=rtr[k].ip;
                m[str]=2;
                rtr[k++].cost=atoi(cost);
                f=0;
            }
            memset(s,'\0',15);
            memset(dest,'\0',15);
            memset(cost,'\0',15);
        }
    }
    s[j]='\0';
    j=0;
    strcpy(cost,s);
    if(f==1){
        strcpy(rtr[k].ip,dest);
	dd[rtr[k].ip]=-1;
        strcpy(rtr[k].nxt,dest);
        str=rtr[k].ip;
        m[str]=2;
        rtr[k++].cost=atoi(cost);
        f=0;
    }
    memset(s,'\0',15);
    memset(dest,'\0',15);
    memset(cost,'\0',15);
    fclose(fptr);
    for(int i=0;i<v.size();i++){
        if(m[v[i]]!=2){
            char s[15];
            int l;
            for(l=0;l<12;l++)s[l]=v[i][l];
            s[l]='\0';
            strcpy(rtr[k].ip,s);
            strcpy(rtr[k].nxt,"inf");
            if(strcmp1(ip,s)==0){rtr[k++].cost=0;}
            else rtr[k++].cost=inf;
        }
    }

}

void print_table(struct router rtr[5])
{
    for(int i=0;i<k;i++){
        rtr[i].ip[12]='\0';
        rtr[i].nxt[12]='\0';
        printf("%s %s %d\n",rtr[i].ip,rtr[i].nxt,rtr[i].cost);
        string s1=rtr[i].ip,s2=thisip;
	//temp1[thisip][s1]=rtr[i].cost;
	//temp1[s1][thisip]=rtr[i].cost;
	if(rtr[i].cost!=inf && rtr[i].cost!=0){
		str=rtr[i].ip;
		if(m["nghbr"]==0){
		//dd[str]=-1;
		nghbr.push_back(str);
		temp1[thisip][str]=rtr[i].cost;
		temp1[str][thisip]=rtr[i].cost;
		}
	}
    }
    m["nghbr"]=1;

}
void update(char sender[15],struct router rtp[5])
{
    char s[15],p[15],q[15],d[15],mn[15];
    int df,gh,ij;
    for(int i=0;i<k;i++){
        strcpy(s,rtr[i].ip);
        ij=rtr[i].cost;
        for(int j=0;j<k;j++){
            strcpy(p,rtp[j].ip);
            //d=rtp[j].nxt;
            gh=rtp[j].cost;
            if(strcmp1(s,p)==0){
                break;
            }
        }
        int l;
        strcpy(d,sender);
        for(l=0;l<k;l++){
            strcpy(q,rtr[l].ip);
            if(strcmp1(q,d)==0){
                df=rtr[l].cost;
		strcpy(mn,rtr[l].nxt);
                break;
            }
        }
	//cout<<dd[s]<<" "<<s<<" "<<df+gh<<endl;
	if(df+gh<ij){
        //if(df+gh<ij && dd[s]==-1){
            strcpy(rtr[i].nxt,mn);
            rtr[i].cost=df+gh;
	    //copy(rtr,temp);
        }
	
    }
}
void up(char *ip2)
{
    char s[15];
    for(int i=0;i<k;i++){
        strcpy(s,rtr[i].ip);
        if(strcmp1(s,ip2)==0){
	    string s1=thisip,s2=ip2;
            rtr[i].cost=temp1[s1][s2];
	    //cout<<rtr[i].cost<<endl;
	    strcpy(rtr[i].nxt,ip2);
            break;
        }
    }
}
void down(char *ip2)
{
    char s[15];
    for(int i=0;i<k;i++){
        strcpy(s,rtr[i].ip);
        if(strcmp1(s,ip2)==0){	
	    string s1=thisip,s2=ip2;;
	    //temp[s1][s2]=rtr[i].cost;
	    
            rtr[i].cost=inf;
	    strcpy(rtr[i].nxt,"inf");
            break;
        }
    }
}
void cost(char *ip2,int a)
{
    char s[15];
    for(int i=0;i<k;i++){
        strcpy(s,rtr[i].ip);
        if(strcmp1(s,ip2)==0){
            rtr[i].cost=a;
	    temp1[thisip][s]=a;
	    temp1[s][thisip]=a;
	    strcpy(rtr[i].nxt,ip2);
            break;
        }
    }
}
char * nxthop(char *ip2)
{
    char s[15];
    for(int i=0;i<k;i++){
        strcpy(s,rtr[i].ip);
        if(strcmp1(s,ip2)==0){
            return rtr[i].nxt;
            break;
        }
    }

}
void sendTable()
{
	string clk="rtrs ";
	for(int i =0;i<k;i++){
		clk.append(rtr[i].ip);
		clk.append(" ");
		//cout<<clk;
		clk.append(rtr[i].nxt);
		clk.append(" ");
		//cout<<clk;
		stringstream ss; 
		ss << rtr[i].cost;
		string str = ss.str();
		clk.append(str);
		clk.append(" ");
	}
	char kk[1024];
	strcpy(thisip, strthisip.c_str());
	strcpy(kk, clk.c_str());		
	inet_pton(AF_INET,thisip,&client_address.sin_addr);

	for(int i=0;i<nghbr.size();i++){
		char ll[1024];
		strcpy(ll,nghbr[i].c_str());
		//printf("%s\n",ll);
		inet_pton(AF_INET,nghbr[i].c_str(),&server_address.sin_addr);
		int sent_bytes=sendto(sockfd,kk, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	}


}


int main(int argc, char *argv[]){

	
	int bind_flag;
	int bytes_received;
    	
    	char * buffer;
    	buffer=(char *) malloc(1024);
	strthisip=argv[1];
	strcpy(thisip,strthisip.c_str());
	if(argc != 2){
		printf("%s <ip address>\n", argv[0]);
		exit(1);
	}
	init(argv[1]);
	print_table(rtr);
	pthread_t a;
	pthread_t b;
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(4747);
	inet_pton(AF_INET,argv[1], &server_address.sin_addr);


	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bind_flag = bind(sockfd, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
	if(bind_flag==0)printf("successful bind\n");

	while(true){
		char command[4],ip[15],ip1[15],ip2[15],msg[1024],l[6];
        	bytes_received = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &server_address, &addrlen);
		for(int i=0;i<4;i++)command[i]=buffer[i];
		command[4]='\0';
		printf("%s\n",command);
		memset(msg,'\0',1024);
		if(strcmp(command,"send")==0){
			char *ip4;
			inet_ntop(AF_INET,buffer+4,ip,sizeof(ip));
			//printf("%s ",ip);
				
                	inet_ntop(AF_INET,buffer+8,ip,sizeof(ip));
			//printf("%s ",ip);
                        int i;
			//for(i=12;i<14;i++)l[i-12]=buffer[i];
			//l[i-12]='\0';
			//printf("%d %s",atoi(l),l);
			int p=static_cast<int>(static_cast<unsigned char>(buffer[12]));
			for(i=14;i<14+p;i++)msg[i-14]=buffer[i];
			msg[i-14]='\0';
			//for(i=1023;i>=0;i--)if(msg[i]==' ')continue;
			//msg[i]='\0';

			ip4=nxthop(ip);

			if(strcmp(ip4,"inf")!=0){
				printf("%s packet forwarded to %s\n",msg,ip4);
				inet_pton(AF_INET,ip4,&server_address.sin_addr);
				string s1=ip;
				stringstream ss; //converting int to string
				ss << p;
				string s2 = ss.str();
				string s3=msg;
				string s="frwd "+s1+" "+s2+" "+s3;
				strcpy(msg,s.c_str());
				//printf("%sdfg\n",msg);
				int sent_bytes=sendto(sockfd,msg, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
			}
			else if(strcmp(ip4,"inf")==0){
				printf("packet can't be sent to the destination");
			}
			else if(strcmp(ip4,ip)==0){
				printf("%s received",msg);
			}
	
		}
		else if(strcmp(command,"frwd")==0){
			char *ip4,num[12];
			int i;
			for(i=5;buffer[i]!=' ';i++)ip[i-5]=buffer[i];
			ip[i-5]='\0';
			//printf("%s\n",ip);

			int j=i;
			i++;
                        for(;buffer[i]!=' ';i++)num[i-j-1]=buffer[i];
			num[i-j-1]='\0';
			int p=atoi(num);
			memset(num,'\0',12);
			j=i;
			i++;
			int h;
			for(h=0;h<p;h++)msg[h]=buffer[i++];
			msg[h]='\0';
			//for(i=1023;i>=0;i--)if(msg[i]==' ')continue;
			//msg[i]='\0';

			//printf("sds%dsds\n",p);
			ip4=nxthop(ip);
			//printf("%s\n",ip4);
			if(strcmp(argv[1],ip)==0){
				printf("%s received",msg);
			}
			else if(strcmp(ip4,"inf")!=0){
				printf("%s packet forwarded to %s\n",msg,ip4);
				//inet_pton(AF_INET,thisip,&client_address.sin_addr);
				string s1=ip;
				stringstream ss; //converting int to string
				ss << p;
				string s2 = ss.str();
				string s3=msg;
				string s="frwd "+s1+" "+s2+" "+s3;
				strcpy(msg,s.c_str());
				inet_pton(AF_INET,ip4,&server_address.sin_addr);
				int sent_bytes=sendto(sockfd,msg, 1024, 0, (struct sockaddr*) &server_address, sizeof(sockaddr_in));
			}
			
			else if(strcmp(ip4,"inf")==0){
				printf("packet can't be sent to the destination");
			}
			
		}
		else if(strcmp(command,"cost")==0){
			inet_ntop(AF_INET,buffer+4,ip1,sizeof(ip));
			printf("%s ",ip1);
				
                	inet_ntop(AF_INET,buffer+8,ip2,sizeof(ip));
			printf("%s ",ip2);
			if(strcmp(argv[1],ip1)==0)strcpy(ip,ip2);
			else strcpy(ip,ip1);
                        int i;
			int p=static_cast<int>(static_cast<unsigned char>(buffer[12]));
			int d=static_cast<int>(static_cast<unsigned char>(buffer[13]));
			d=d<<8;
			p+=d;
			//printf("%d",p);
			cost(ip,p);
			print_table(rtr);
		}
		else if(strcmp(command,"show")==0){
			inet_ntop(AF_INET,buffer+4,ip,sizeof(ip));
			print_table(rtr);
				
             
		}
		else if(strcmp(command,"clk ")==0){
			sendTable();
			for(int i=0;i<nghbr.size();i++){
				if(dd[nghbr[i]]==0){
					cnt[nghbr[i]]++;	
				}
				else if(dd[nghbr[i]]==-1){
					cnt[nghbr[i]]=0;	
				}
				if(cnt[nghbr[i]]>=3){
					//cnt[nghbr[i]]++;	
					printf("The link between %s and %s is down\n",nghbr[i].c_str(),thisip);
					char rr[15];
					strcpy(rr,nghbr[i].c_str());
					down(rr);
				}
				dd[nghbr[i]]=0;
			}
				
             
		}
		else if(strcmp(command,"rtrs")==0){
			string sender;
			string clk;
			int neighbour_cost;
			sender=inet_ntoa(server_address.sin_addr);
			char sndr[15];
			strcpy(sndr,sender.c_str());
			clk=buffer;
			cout<<"table received from "<<sender<<endl;
			
			//Detection of down
			dd[sender]=-1;
			if(cnt[sender]>=3){
				//dd[sender]=-1;
				cnt[sender]=0;
				printf("The link between %s and %s is up again\n",sender.c_str(),thisip);
			
				char rr[15];
				strcpy(rr,sender.c_str());
				up(rr);
				
			}
			/*for(int i=0;i<nghbr.size();i++){
				if(cnt[nghbr[i]]==3){
					//cnt[nghbr[i]]++;	
					printf("The link between %s and %s has gone down",nghbr[i],thisip);
					down(nghbr[i]);
				}
				else if(dd[nghbr[i]]==-1){
					cnt[nghbr[i]]=0;	
				}
				
			}*/
			


			istringstream iss(clk);
			vector<string> routerrecv;
			copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(routerrecv));
			//for(int i =0 ;i<routerrecv.size();i++){
				//cout<<"routerrecv "<<i<<" = "<<routerrecv[i]<<endl;
			//}


			for(int l=1,h=0;l<routerrecv.size();){
				strcpy(rtp[h].ip,routerrecv[l].c_str());
				strcpy(rtp[h].nxt,routerrecv[l+1].c_str());
				int val;
				char fg[15];
				strcpy(fg,routerrecv[l+2].c_str());
				val=atoi(fg);
				rtp[h++].cost=val;
				l+=3;
			}	
			//print_table(rtp);
			update(sndr,rtp);
		}
                printf("\n");
		//if(strcmp(buffer,"exit")==0)break;
	}

	close(sockfd);


	return 0;

}
