


class IDevice
{
    const char* id;
    const char* type;
    public:
        IDevice(const char* id, const char* type):id(id), type(type){}
        virtual ~IDevice(){}
        const char* get_id() const {return id;}
        const char* get_type() const {return type;}
};

class IInput: public virtual IDevice
{
    public:
    IInput(const char* id, const char* type):IDevice(id, type){}
    virtual long get_value(unsigned short index) const = 0;
};

class IOutput: public virtual IDevice
{
    public:
    IOutput(const char* id, const char* type):IDevice(id, type){}
    virtual bool set_value(unsigned short index, long value) = 0;
    virtual bool increase_value(unsigned short index, long value) =0;
    virtual bool decrease_value(unsigned short index, long value) =0;
};
