//
//  LoopNode.h
//
//

#ifndef LoopNode_h
#define LoopNode_h
#include "Node.h"

//Looping over variables
class LoopNode : public Node{
private:
    double (*f)(std::vector<double>);
    std::vector <Node*> lefs;

protected:
    std::vector<myParticle*> inputParticles;

public:
    LoopNode(double (*func)(std::vector<double>), Node* l, std::string s){
        f=func;
        symbol=s;
        left=l;
        lefs.push_back(l);
        right=NULL;
    }
    LoopNode(double (*func)(std::vector<double>), std::vector<Node*> ls, std::string s){
        f=func;
        symbol=s;
        left=ls[0]; // just in case if someone asks the list of particles for example
        lefs=ls;
        right=NULL;
    }
    
    virtual void getParticles(std::vector<myParticle *>* particles) override{
         left->getParticles(particles);
    }

    virtual void getParticlesAt(std::vector<myParticle *>* particles, int index) override{
        left->getParticlesAt(particles,index);
    }

    virtual void Reset() override{
        left->Reset();
    }

    virtual double evaluate(AnalysisObjects* ao) override;
    
    virtual ~LoopNode() {
        if (left!=NULL) delete left;
    }
    
};

double sumof(std::vector<double> xlist);
double minof(std::vector<double> xlist);
double maxof(std::vector<double> xlist);


#endif /* LoopNode_h */
