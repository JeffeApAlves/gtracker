//
// Generated file, do not edit! Created by nedtool 5.2 from veins/modules/messages/PhyControlMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __PHYCONTROLMESSAGE_M_H
#define __PHYCONTROLMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0502
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>veins/modules/messages/PhyControlMessage.msg:22</tt> by nedtool.
 * <pre>
 * //
 * // Defines a control message that can be associated with a MAC frame to set
 * // transmission power and datarate on a per packet basis
 * //
 * message PhyControlMessage
 * {
 *     //modulation and coding scheme to be used (see enum TxMCS in ConstsPhy.h)
 *     int mcs = -1;
 *     //transmission power to be used in mW
 *     double txPower_mW = -1;
 * }
 * </pre>
 */
class PhyControlMessage : public ::omnetpp::cMessage
{
  protected:
    int mcs;
    double txPower_mW;

  private:
    void copy(const PhyControlMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PhyControlMessage&);

  public:
    PhyControlMessage(const char *name=nullptr, short kind=0);
    PhyControlMessage(const PhyControlMessage& other);
    virtual ~PhyControlMessage();
    PhyControlMessage& operator=(const PhyControlMessage& other);
    virtual PhyControlMessage *dup() const override {return new PhyControlMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getMcs() const;
    virtual void setMcs(int mcs);
    virtual double getTxPower_mW() const;
    virtual void setTxPower_mW(double txPower_mW);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const PhyControlMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, PhyControlMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PHYCONTROLMESSAGE_M_H

