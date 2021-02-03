#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;
ofstream file;
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

class Path{
  private:
  vector<Node> wps;
  vector<Node> injWps;
  double spacing;

  vector<Node> smoWps;
  double w_data, w_smooth, tolerance;

  vector<double> dist;
  vector<double> maxVel;
  public:
  Path(): wps{}, dist{}, maxVel{}
  {}
  Path(vector<Node> p_wps): wps{p_wps}, dist{}, maxVel{}
  {}
  vector<Node> getWps(){
    return wps;
  }
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
  void setWps(vector<Node> p_wps, double p_spacing, double p_w_data, double p_w_smooth, double p_tolerance){
    //perform injection and smooth
    wps = p_wps;
    spacing = p_spacing;
    w_data = p_w_data;
    w_smooth = p_w_smooth;
    tolerance = p_tolerance;
    this->inject();
    this->smooth();
  }
  void outputToText(){
    file << spacing << ' ' << w_data << ' ' << w_smooth << '\t' << tolerance << ' ' << smoWps.size() << ' ' << wps.size() << '\n';

    for(auto smoWp : smoWps){
      file << smoWp.getX() << ' ' << smoWp.getY() << '\n';
    }
    for(auto wp : wps){
      file << wp.getX() << ' ' << wp.getY() << '\n';
    }
    file.close();
  }
};

int main(){
  Path path;
  file.open("plot.txt", ios::out | ios::trunc);
  path.setWps({{0, 0}, {90, 0}, {135, -50}},6, .25, .75, .001);
  path.outputToText();


  file.close();
  return 0;
}