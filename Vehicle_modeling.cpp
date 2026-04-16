#inlcude<iostream>
#include<vector>
#include<memory>
#include<cmath>

using namespace std; 

class Vehicle{
public: 
   double mass; 
   double position; 
   double velocity; 
   Vehicle(double m): mass(m), position(0.0), velocity(0.0); 
};

class forceModel(){
  public: 
  virtual double computeForce(Vehicle& v)=0; 
  virtual ~forceModel()=defualt; 
}
class engineForce: public forceModel{
private: 
   double throttle; 
   double maxForce; 
public: 
engineForce(double maxf): throttle(0.0), maxForce(maxf){}
void setThrottle(double t){
  throttle=fmax(0.0, fmin(1.0, t)); 
}
double computeForce(const Vehicle& v) override {
  return throttle*maxForce; 
}
};
class dragForce: public forceModel{
private: 
   double dragCoff; 
public: 
   dragForce(double cd): dragCoff(cd){}
   double computeForce(const Vehicle& v) override{
     double direction=(v.velocity>=0)? -1:1; 
     return direction*dragCoff*v.velocity*v.velocity; 
   }
};
class simulationVehicle{
private:
   Vehicle state; 
   vector<unique_ptr<forceModel>> forces; 
public: 
  simulationVehicle(double mass): state(mass){}
  addForce(uinque_ptr<forceModel> f){
    forces.push_back(move(f)); 
  }
  void step(double dt){
    double totalForce=0.0; 
    for(auto f:forces){
      totalForce+=f->computeForce(state); 
    }
    double acceleration = totalForce/state.mass; 
    state.velocity+=acceleration*dt; 
    state.position+=state.velocity*dt; 
  }
};
int main(){
  simulationVehicle car(1500); 
  auto engine=engineForce(500); 
  auto drag=dragForce(0.4); 
  engine.setThrottle(0.8); 
  car.addForce(engine); 
  car.addForce(drag); 
  
  double dt=0.1; 
  for(int i=0;i<100; ++i){
    car.step(dt); 
    cout << i*dt << car.state.velocity << car.state.position << endl; 
  }
  
  return 0; 
}
