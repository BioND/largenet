
#ifndef SUGGESTIONPOWER_H_
#define SUGGESTIONPOWER_H_

#include "IntervalOutput.h"
#include <largenet.h>
#include <largenet/TripleNetwork.h>


class SuggestionPower: public IntervalOutput
{
public:
	SuggestionPower(std::ostream& out, const lnet::TripleNetwork& net, double interval);
	virtual ~SuggestionPower();
private:
	void doOutput(double t);
	void doWriteHeader();
	const lnet::TripleNetwork& net_;
};

#endif /* SUGGESTIONPOWER_H_ */
