class Product
{
public:
    virtual ~Product(){};
    virtual void Operation(){};
};

class ConcreteProductA : public Product{
public:
    void Operation(){
        return ;
    }
};

class ConcreteProductB : public Product{
public:
    void Operation(){
        return ;
    }
};

class Creator{
public:
    virtual ~Creator(){};
    virtual Product* FactoryMethod() const = 0;
    void SomeOperation() const {
        Product* product = this->FactoryMethod();
        delete product;
        return;
    }
};

class ConcreteCreatorA : public Creator{
public:
    Product* FactoryMethod() const override {
        return new ConcreteProductA();
    }

};

class ConcreteCreatorB : public Creator{
public:
    Product* FactoryMethod() const override {
        return new ConcreteProductB();
    }
    
};



int main()
{
    ConcreteCreatorA A;
    A.SomeOperation();
    ConcreteCreatorB B;
    B.SomeOperation();
    return 0;
}