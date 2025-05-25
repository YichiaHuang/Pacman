#ifndef TOOL_HPP
#define TOOL_HPP

class Tool {
public:
    virtual void Use(int x, int y) = 0;
    virtual const char* GetName() const = 0;
    virtual ~Tool() = default;
};

#endif
