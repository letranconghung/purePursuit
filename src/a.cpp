#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;
#define INF 1e10
#define INFsmall 1/INF
#define MAX_POW 100
ofstream pathTextFile;
ofstream kinematicsTextFile;
/**
 * class Node
 * */
class Node{
  private:
  double x, y;
  public:
  Node(): x{0}, y{0}
  {}
  Node(double p_x, double p_y): x{p_x}, y{p_y}
  {}
  friend Node operator+(const Node &n1, const Node &n2);
  friend Node operator-(const Node &n1, const Node &n2);
  friend Node operator*(const Node &n, double val);
  friend Node operator/(const Node &n, double val);
  pair<double, double> getXY(){
    return make_pair(x, y);
  }
  double getX(){
    return x;
  }
  double getY(){
    return y;
  }
  void setXY(double x, double y){
    this -> x = x;
    this -> y = y;
  }
  void print(){
    printf("X: %.2f Y: %.2f\n", x, y);
  }
  double mag(){
    return sqrt(pow(this-> x, 2) + pow(this-> y, 2)); 
  }
  Node norm(){
    double mag = this->mag();
    return {(this->x)/mag, (this->y)/mag};
  }
};
Node operator+(const Node &n1, const Node &n2){
  return {n1.x + n2.x, n1.y + n2.y};
}
Node operator-(const Node &n1, const Node &n2){
  return {n1.x - n2.x, n1.y - n2.y};
}
Node operator*(const Node &n, double val){
  return {n.x*val, n.y*val};
}
Node operator/(const Node &n, double val){
  return {n.x/val, n.y/val};
}
/**
 * Math utility functions
 * */
double distance(Node n1, Node n2){
  Node diff = n2 - n1;
  return diff.mag();
}
double circumRad(Node n1, Node n2, Node n3){
  double a = distance(n1, n2);
  double b = distance(n2, n3);
  double c = distance(n3, n1);
  double denom = sqrt((a+b+c)*(b+c-a)*(c+a-b)*(a+b-c));
  if(denom <= INFsmall) return INF;
  else return a*b*c/denom;
}
/**
 * Debugging utility functions
 * */
void printVector(vector<double> vd){
  for(auto m : vd){
    printf("%.2f ", m);
  }
  cout << endl;
}
/**
 * class Path
 * */
class Path{
  private:
  vector<Node> wps;
  vector<Node> injWps;
  double spacing;

  vector<Node> smoWps;
  double w_data, w_smooth, tolerance;

  vector<double> dist;
  vector<double> curv;
  vector<double> maxV;
  double maxVel;
  public:
  Path(): wps{}, dist{}, maxV{}
  {}
  Path(vector<Node> p_wps): wps{p_wps}, dist{}, maxV{}
  {}
  void inject(){
    injWps.clear();
    for(int i = 0;i<=wps.size()-2;++i){
      Node start = wps[i];
      Node end = wps[i+1];
      Node diff = end - start;
      Node step = diff.norm()*spacing;
      int nFit = ceil(diff.mag()/spacing);
      for(int j = 0;j<nFit;++j){
        injWps.push_back(start + step*j);
      }
    }
    injWps.push_back(wps[wps.size()-1]);
  }
  void smooth(){
    smoWps = injWps;
    double change = tolerance;
    while(change >= tolerance){
      change = 0;
      for(int i = 1;i<injWps.size()-1;++i){
        Node aux = smoWps[i];
        smoWps[i] = aux + (injWps[i] - smoWps[i])*w_data + (smoWps[i-1] + smoWps[i+1] - smoWps[i]*2)*w_smooth;
        
        Node diff = smoWps[i] - aux;
        change += (fabs(diff.getX()) + fabs(diff.getY()));
      }
      printf("change = %.5f\n", change);
    }
  }
  void calcDist(){
    Node prevWp = smoWps[0];
    double prevDist = 0;
    for(auto smoWp : smoWps){
      double deltaDist = distance(smoWp, prevWp);
      double newDist = prevDist + deltaDist;
      dist.push_back(newDist);
      prevDist = newDist;
      prevWp = smoWp;
    }
    printVector(dist);
  }
  void calcCurvature(){
    curv.push_back(INFsmall); //starting point
    for (int i = 1;i<=smoWps.size()-2;++i){
      double r = circumRad(smoWps[i-1], smoWps[i], smoWps[i+1]);
      curv.push_back(1/r);
    }
    curv.push_back(INFsmall); //final point
  }
  void calcMaxV(double maxVel, double k){
    for(int i = 0;i<smoWps.size();++i){
      maxV.push_back(min(maxVel, k/curv[i]));
    }
    printVector(maxV);
  }
  void setWps(vector<Node> p_wps, double p_spacing, double p_w_data, double p_w_smooth, double p_tolerance, double p_maxVel, double p_k){
    //perform injection and smooth
    wps = p_wps;
    spacing = p_spacing;
    w_data = p_w_data;
    w_smooth = p_w_smooth;
    tolerance = p_tolerance;
    maxVel = p_maxVel;
    this->inject();
    this->smooth();
    this->calcDist();
    this->calcCurvature();
    this->calcMaxV(p_maxVel, p_k);
  }
  void outputPathToText(){
    pathTextFile << spacing << ' ' << w_data << ' ' << w_smooth << '\t' << tolerance << ' ' << smoWps.size() << ' ' << wps.size() << '\n';
    for(auto smoWp : smoWps){
      pathTextFile << smoWp.getX() << ' ' << smoWp.getY() << '\n';
    }
    for(auto wp : wps){
      pathTextFile << wp.getX() << ' ' << wp.getY() << '\n';
    }
  }
  void outputKinematicsToText(){
    for(int i = 0;i<smoWps.size();++i){
      kinematicsTextFile << dist[i] << ' ' << maxV[i] << '\n';
    }
  }
};

int main(){
  Path path;
  pathTextFile.open("../txt/path.txt", ios::out | ios::trunc);
  kinematicsTextFile.open("../txt/kinematics.txt", ios::out | ios::trunc);
  /**
   * initialization code
   * */

  path.setWps({{0, 0}, {90, 0}, {135, -50}},6, .25, .75, .001, 100, 0.5);
  path.outputPathToText();
  path.outputKinematicsToText();
  /**
   * finalization code
   * */
  pathTextFile.close();
  kinematicsTextFile.close();
  return 0;
}