#ifndef __FIXTURE_USER_DATA_H__
#define __FIXTURE_USER_DATA_H__

//types of fixture user data
enum fixtureUserDataType {
    FUD_CAR_TIRE,
    FUD_GROUND_AREA
};

//a class to allow subclassing of different fixture user data
class FixtureUserData {
    private:
    fixtureUserDataType m_type;
    
    protected:
    FixtureUserData(fixtureUserDataType type): m_type(type) {}
    
    public:
    virtual fixtureUserDataType getType() { return m_type; }
    virtual ~FixtureUserData() {}
};

//class to allow marking a fixture as a car tire
class CarTireFUD : public FixtureUserData {
public:
    CarTireFUD(): FixtureUserData(FUD_CAR_TIRE) {}
};

//class to allow marking a fixture as a ground area
class GroundAreaFUD : public FixtureUserData {
public:
    float frictionModifier;
    bool outOfCourse;

    GroundAreaFUD(float fm, bool ooc) : FixtureUserData(FUD_GROUND_AREA) {
        frictionModifier = fm;
        outOfCourse = ooc;
    }
};

#endif
