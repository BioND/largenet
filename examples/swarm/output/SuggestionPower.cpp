
#include "SuggestionPower.h"
#include "../SwarmModel.h"
#include <iomanip>

using namespace std;
using namespace lnet;

SuggestionPower::SuggestionPower(ostream& out, const TripleNetwork& net,
		const double interval) :
		IntervalOutput(out, interval), net_(net)
{
}

SuggestionPower::~SuggestionPower()
{
}

void SuggestionPower::doWriteHeader()
{
	const char tab = '\t';
	stream() << commentChar() << 't';

	stream() << tab << "R_r";
	stream() << tab << "R_r - 1";
	stream() << tab << "R_r - 2";
	stream() << tab << "R_r - 3";
	stream() << tab << "R_r - 4";
	stream() << tab << "R_r - 5";

	stream() << tab << "R_u";
	stream() << tab << "R_u - 1";
	stream() << tab << "R_u - 2";
	stream() << tab << "R_u - 3";
	stream() << tab << "R_u - 4";
	stream() << tab << "R_u - 5";

	stream() << tab << "R_l";
	stream() << tab << "R_l - 1";
	stream() << tab << "R_l - 2";
	stream() << tab << "R_l - 3";
	stream() << tab << "R_l - 4";
	stream() << tab << "R_l - 5";

	stream() << tab << "L_r";
	stream() << tab << "L_r - 1";
	stream() << tab << "L_r - 2";
	stream() << tab << "L_r - 3";
	stream() << tab << "L_r - 4";
	stream() << tab << "L_r - 5";

	stream() << tab << "L_u";
	stream() << tab << "L_u - 1";
	stream() << tab << "L_u - 2";
	stream() << tab << "L_u - 3";
	stream() << tab << "L_u - 4";
	stream() << tab << "L_u - 5";

	stream() << tab << "L_l";
	stream() << tab << "L_l - 1";
	stream() << tab << "L_l - 2";
	stream() << tab << "L_l - 3";
	stream() << tab << "L_l - 4";
	stream() << tab << "L_l - 5";



	stream() << "\n";
}

void SuggestionPower::doOutput(const double t)
{
	const char sep = '\t';
	stream() << t;

	TripleNetwork::NodeStateIteratorRange iters = net_.nodes(SwarmModel::Rrinf);
	double RrPower1 = 0;
	double RrPower2 = 0;
	double RrPower3 = 0;
	double RrPower4 = 0;
	double RrPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double RrRNegh1 = 0;
		double RrNegh1 = 0;
		double RrRNegh2 = 0;
		double RrNegh2 = 0;
		double RrRNegh3 = 0;
		double RrNegh3 = 0;
		double RrRNegh4 = 0;
		double RrNegh4 = 0;
		double RrRNegh5 = 0;
		double RrNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if((net_.nodeState(*nit)==SwarmModel::Rrinf) || (net_.nodeState(*nit)==SwarmModel::Rlinf) || (net_.nodeState(*nit)==SwarmModel::Runinf))
				RrRNegh1 ++;
			RrNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if((net_.nodeState(*nit2)==SwarmModel::Rrinf) || (net_.nodeState(*nit2)==SwarmModel::Rlinf) || (net_.nodeState(*nit2)==SwarmModel::Runinf))
						RrRNegh2 ++;
					RrNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if((net_.nodeState(*nit3)==SwarmModel::Rrinf) || (net_.nodeState(*nit3)==SwarmModel::Rlinf) || (net_.nodeState(*nit3)==SwarmModel::Runinf))
								RrRNegh3 ++;
							RrNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if((net_.nodeState(*nit4)==SwarmModel::Rrinf) || (net_.nodeState(*nit4)==SwarmModel::Rlinf) || (net_.nodeState(*nit4)==SwarmModel::Runinf))
										RrRNegh4 ++;
									RrNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if((net_.nodeState(*nit5)==SwarmModel::Rrinf) || (net_.nodeState(*nit5)==SwarmModel::Rlinf) || (net_.nodeState(*nit5)==SwarmModel::Runinf))
												RrRNegh5 ++;
											RrNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (RrNegh5 > 0)
			RrPower5 += RrRNegh5/RrNegh5;
		if (RrNegh4 > 0)
			RrPower4 += RrRNegh4/RrNegh4;
		if (RrNegh3 > 0)
			RrPower3 += RrRNegh3/RrNegh3;
		if (RrNegh2 > 0)
			RrPower2 += RrRNegh2/RrNegh2;
		if (RrNegh1 > 0)
			RrPower1 += RrRNegh1/RrNegh1;
	}
	stream() << sep << std::setprecision(5) << net_.numberOfNodes(SwarmModel::Rrinf);
	stream() << sep << std::setprecision(5) << RrPower1 / net_.numberOfNodes(SwarmModel::Rrinf);
	stream() << sep << std::setprecision(5) << RrPower2 / net_.numberOfNodes(SwarmModel::Rrinf);
	stream() << sep << std::setprecision(5) << RrPower3 / net_.numberOfNodes(SwarmModel::Rrinf);
	stream() << sep << std::setprecision(5) << RrPower4 / net_.numberOfNodes(SwarmModel::Rrinf);
	stream() << sep << std::setprecision(5) << RrPower5 / net_.numberOfNodes(SwarmModel::Rrinf);


	iters = net_.nodes(SwarmModel::Runinf);
	double RuPower1 = 0;
	double RuPower2 = 0;
	double RuPower3 = 0;
	double RuPower4 = 0;
	double RuPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double RuRNegh1 = 0;
		double RuNegh1 = 0;
		double RuRNegh2 = 0;
		double RuNegh2 = 0;
		double RuRNegh3 = 0;
		double RuNegh3 = 0;
		double RuRNegh4 = 0;
		double RuNegh4 = 0;
		double RuRNegh5 = 0;
		double RuNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if(net_.nodeState(*nit)==SwarmModel::Rrinf || net_.nodeState(*nit)==SwarmModel::Rlinf || net_.nodeState(*nit)==SwarmModel::Runinf)
				RuRNegh1 ++;
			RuNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if(net_.nodeState(*nit2)==SwarmModel::Rrinf || net_.nodeState(*nit2)==SwarmModel::Rlinf || net_.nodeState(*nit2)==SwarmModel::Runinf)
						RuRNegh2 ++;
					RuNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if(net_.nodeState(*nit3)==SwarmModel::Rrinf || net_.nodeState(*nit3)==SwarmModel::Rlinf || net_.nodeState(*nit3)==SwarmModel::Runinf)
								RuRNegh3 ++;
							RuNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if(net_.nodeState(*nit4)==SwarmModel::Rrinf || net_.nodeState(*nit4)==SwarmModel::Rlinf || net_.nodeState(*nit4)==SwarmModel::Runinf)
										RuRNegh4 ++;
									RuNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if(net_.nodeState(*nit5)==SwarmModel::Rrinf || net_.nodeState(*nit5)==SwarmModel::Rlinf || net_.nodeState(*nit5)==SwarmModel::Runinf)
												RuRNegh5 ++;
											RuNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (RuNegh5 > 0)
			RuPower5 += RuRNegh5/RuNegh5;
		if (RuNegh4 > 0)
			RuPower4 += RuRNegh4/RuNegh4;
		if (RuNegh3 > 0)
			RuPower3 += RuRNegh3/RuNegh3;
		if (RuNegh2 > 0)
			RuPower2 += RuRNegh2/RuNegh2;
		if (RuNegh1 > 0)
			RuPower1 += RuRNegh1/RuNegh1;
	}
	stream() << sep << std::setprecision(5) << net_.numberOfNodes(SwarmModel::Runinf);
	stream() << sep << std::setprecision(5) << RuPower1/net_.numberOfNodes(SwarmModel::Runinf);
	stream() << sep << std::setprecision(5) << RuPower2/net_.numberOfNodes(SwarmModel::Runinf);
	stream() << sep << std::setprecision(5) << RuPower3/net_.numberOfNodes(SwarmModel::Runinf);
	stream() << sep << std::setprecision(5) << RuPower4/net_.numberOfNodes(SwarmModel::Runinf);
	stream() << sep << std::setprecision(5) << RuPower5/net_.numberOfNodes(SwarmModel::Runinf);

	iters = net_.nodes(SwarmModel::Rlinf);
	double RlPower1 = 0;
	double RlPower2 = 0;
	double RlPower3 = 0;
	double RlPower4 = 0;
	double RlPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double RlRNegh1 = 0;
		double RlNegh1 = 0;
		double RlRNegh2 = 0;
		double RlNegh2 = 0;
		double RlRNegh3 = 0;
		double RlNegh3 = 0;
		double RlRNegh4 = 0;
		double RlNegh4 = 0;
		double RlRNegh5 = 0;
		double RlNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if(net_.nodeState(*nit)==SwarmModel::Rrinf || net_.nodeState(*nit)==SwarmModel::Rlinf || net_.nodeState(*nit)==SwarmModel::Runinf)
				RlRNegh1 ++;
			RlNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if(net_.nodeState(*nit2)==SwarmModel::Rrinf || net_.nodeState(*nit2)==SwarmModel::Rlinf || net_.nodeState(*nit2)==SwarmModel::Runinf)
						RlRNegh2 ++;
					RlNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if(net_.nodeState(*nit3)==SwarmModel::Rrinf || net_.nodeState(*nit3)==SwarmModel::Rlinf || net_.nodeState(*nit3)==SwarmModel::Runinf)
								RlRNegh3 ++;
							RlNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if(net_.nodeState(*nit4)==SwarmModel::Rrinf || net_.nodeState(*nit4)==SwarmModel::Rlinf || net_.nodeState(*nit4)==SwarmModel::Runinf)
										RlRNegh4 ++;
									RlNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if(net_.nodeState(*nit5)==SwarmModel::Rrinf || net_.nodeState(*nit5)==SwarmModel::Rlinf || net_.nodeState(*nit5)==SwarmModel::Runinf)
												RlRNegh5 ++;
											RlNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (RlNegh5 > 0)
			RlPower5 += RlRNegh5/RlNegh5;
		if (RlNegh4 > 0)
			RlPower4 += RlRNegh4/RlNegh4;
		if (RlNegh3 > 0)
			RlPower3 += RlRNegh3/RlNegh3;
		if (RlNegh2 > 0)
			RlPower2 += RlRNegh2/RlNegh2;
		if (RlNegh1 > 0)
			RlPower1 += RlRNegh1/RlNegh1;
	}
	stream() << sep << std::setprecision(5) << net_.numberOfNodes(SwarmModel::Rlinf);
	stream() << sep << std::setprecision(5) << RlPower1/net_.numberOfNodes(SwarmModel::Rlinf);
	stream() << sep << std::setprecision(5) << RlPower2/net_.numberOfNodes(SwarmModel::Rlinf);
	stream() << sep << std::setprecision(5) << RlPower3/net_.numberOfNodes(SwarmModel::Rlinf);
	stream() << sep << std::setprecision(5) << RlPower4/net_.numberOfNodes(SwarmModel::Rlinf);
	stream() << sep << std::setprecision(5) << RlPower5/net_.numberOfNodes(SwarmModel::Rlinf);

	iters = net_.nodes(SwarmModel::Lrinf);
	double LrPower1 = 0;
	double LrPower2 = 0;
	double LrPower3 = 0;
	double LrPower4 = 0;
	double LrPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double LrLNegh1 = 0;
		double LrNegh1 = 0;
		double LrLNegh2 = 0;
		double LrNegh2 = 0;
		double LrLNegh3 = 0;
		double LrNegh3 = 0;
		double LrLNegh4 = 0;
		double LrNegh4 = 0;
		double LrLNegh5 = 0;
		double LrNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if(net_.nodeState(*nit)==SwarmModel::Lrinf || net_.nodeState(*nit)==SwarmModel::Llinf || net_.nodeState(*nit)==SwarmModel::Luninf)
				LrLNegh1 ++;
			LrNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if(net_.nodeState(*nit2)==SwarmModel::Lrinf || net_.nodeState(*nit2)==SwarmModel::Llinf || net_.nodeState(*nit2)==SwarmModel::Luninf)
						LrLNegh2 ++;
					LrNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if(net_.nodeState(*nit3)==SwarmModel::Lrinf || net_.nodeState(*nit3)==SwarmModel::Llinf || net_.nodeState(*nit3)==SwarmModel::Luninf)
								LrLNegh3 ++;
							LrNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if(net_.nodeState(*nit4)==SwarmModel::Lrinf || net_.nodeState(*nit4)==SwarmModel::Llinf || net_.nodeState(*nit4)==SwarmModel::Luninf)
										LrLNegh4 ++;
									LrNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if(net_.nodeState(*nit5)==SwarmModel::Lrinf || net_.nodeState(*nit5)==SwarmModel::Llinf || net_.nodeState(*nit5)==SwarmModel::Luninf)
												LrLNegh5 ++;
											LrNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (LrNegh5 > 0)
			LrPower5 += LrLNegh5/LrNegh5;
		if (LrNegh4 > 0)
			LrPower4 += LrLNegh4/LrNegh4;
		if (LrNegh3 > 0)
			LrPower3 += LrLNegh3/LrNegh3;
		if (LrNegh2 > 0)
			LrPower2 += LrLNegh2/LrNegh2;
		if (LrNegh1 > 0)
			LrPower1 += LrLNegh1/LrNegh1;
	}
	stream() << sep << net_.numberOfNodes(SwarmModel::Lrinf);
	stream() << sep << std::setprecision(5) << LrPower1/net_.numberOfNodes(SwarmModel::Lrinf);
	stream() << sep << std::setprecision(5) << LrPower2/net_.numberOfNodes(SwarmModel::Lrinf);
	stream() << sep << std::setprecision(5) << LrPower3/net_.numberOfNodes(SwarmModel::Lrinf);
	stream() << sep << std::setprecision(5) << LrPower4/net_.numberOfNodes(SwarmModel::Lrinf);
	stream() << sep << std::setprecision(5) << LrPower5/net_.numberOfNodes(SwarmModel::Lrinf);

	iters = net_.nodes(SwarmModel::Luninf);
	double LuPower1 = 0;
	double LuPower2 = 0;
	double LuPower3 = 0;
	double LuPower4 = 0;
	double LuPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double LuLNegh1 = 0;
		double LuNegh1 = 0;
		double LuLNegh2 = 0;
		double LuNegh2 = 0;
		double LuLNegh3 = 0;
		double LuNegh3 = 0;
		double LuLNegh4 = 0;
		double LuNegh4 = 0;
		double LuLNegh5 = 0;
		double LuNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if(net_.nodeState(*nit)==SwarmModel::Lrinf || net_.nodeState(*nit)==SwarmModel::Llinf || net_.nodeState(*nit)==SwarmModel::Luninf)
				LuLNegh1 ++;
			LuNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if(net_.nodeState(*nit2)==SwarmModel::Lrinf || net_.nodeState(*nit2)==SwarmModel::Llinf || net_.nodeState(*nit2)==SwarmModel::Luninf)
						LuLNegh2 ++;
					LuNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if(net_.nodeState(*nit3)==SwarmModel::Lrinf || net_.nodeState(*nit3)==SwarmModel::Llinf || net_.nodeState(*nit3)==SwarmModel::Luninf)
								LuLNegh3 ++;
							LuNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if(net_.nodeState(*nit4)==SwarmModel::Lrinf || net_.nodeState(*nit4)==SwarmModel::Llinf || net_.nodeState(*nit4)==SwarmModel::Luninf)
										LuLNegh4 ++;
									LuNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if(net_.nodeState(*nit5)==SwarmModel::Lrinf || net_.nodeState(*nit5)==SwarmModel::Llinf || net_.nodeState(*nit5)==SwarmModel::Luninf)
												LuLNegh5 ++;
											LuNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (LuNegh5 > 0)
			LuPower5 += LuLNegh5/LuNegh5;
		if (LuNegh4 > 0)
			LuPower4 += LuLNegh4/LuNegh4;
		if (LuNegh3 > 0)
			LuPower3 += LuLNegh3/LuNegh3;
		if (LuNegh2 > 0)
			LuPower2 += LuLNegh2/LuNegh2;
		if (LuNegh1 > 0)
			LuPower1 += LuLNegh1/LuNegh1;
	}
	stream() << sep << net_.numberOfNodes(SwarmModel::Luninf);
	stream() << sep << std::setprecision(5) << LuPower1/net_.numberOfNodes(SwarmModel::Luninf);
	stream() << sep << std::setprecision(5) << LuPower2/net_.numberOfNodes(SwarmModel::Luninf);
	stream() << sep << std::setprecision(5) << LuPower3/net_.numberOfNodes(SwarmModel::Luninf);
	stream() << sep << std::setprecision(5) << LuPower4/net_.numberOfNodes(SwarmModel::Luninf);
	stream() << sep << std::setprecision(5) << LuPower5/net_.numberOfNodes(SwarmModel::Luninf);

	iters = net_.nodes(SwarmModel::Llinf);
	double LlPower1 = 0;
	double LlPower2 = 0;
	double LlPower3 = 0;
	double LlPower4 = 0;
	double LlPower5 = 0;

	for (TripleNetwork::NodeStateIterator it = iters.first; it != iters.second; ++it)
	{
		double LlLNegh1 = 0;
		double LlNegh1 = 0;
		double LlLNegh2 = 0;
		double LlNegh2 = 0;
		double LlLNegh3 = 0;
		double LlNegh3 = 0;
		double LlLNegh4 = 0;
		double LlNegh4 = 0;
		double LlLNegh5 = 0;
		double LlNegh5 = 0;
		TripleNetwork::NeighborIteratorRange neghiters = net_.neighbors(*it);
		for(TripleNetwork::NeighborIterator nit = neghiters.first; nit != neghiters.second; nit++)
		{
			if(net_.nodeState(*nit)==SwarmModel::Lrinf || net_.nodeState(*nit)==SwarmModel::Llinf || net_.nodeState(*nit)==SwarmModel::Luninf)
				LlLNegh1 ++;
			LlNegh1 ++;
			TripleNetwork::NeighborIteratorRange neghiters2 = net_.neighbors(*nit);
			for(TripleNetwork::NeighborIterator nit2 = neghiters2.first; nit2 != neghiters2.second; nit2++)
			{
				if(*nit2 != *it)
				{
					if(net_.nodeState(*nit2)==SwarmModel::Lrinf || net_.nodeState(*nit2)==SwarmModel::Llinf || net_.nodeState(*nit2)==SwarmModel::Luninf)
						LlLNegh2 ++;
					LlNegh2 ++;
					TripleNetwork::NeighborIteratorRange neghiters3 = net_.neighbors(*nit2);
					for(TripleNetwork::NeighborIterator nit3 = neghiters3.first; nit3 != neghiters3.second; nit3++)
					{
						if(*nit3 != *nit)
						{
							if(net_.nodeState(*nit3)==SwarmModel::Lrinf || net_.nodeState(*nit3)==SwarmModel::Llinf || net_.nodeState(*nit3)==SwarmModel::Luninf)
								LlLNegh3 ++;
							LlNegh3 ++;
							TripleNetwork::NeighborIteratorRange neghiters4 = net_.neighbors(*nit3);
							for(TripleNetwork::NeighborIterator nit4 = neghiters4.first; nit4 != neghiters4.second; nit4++)
							{
								if(*nit4 != *nit2)
								{
									if(net_.nodeState(*nit4)==SwarmModel::Lrinf || net_.nodeState(*nit4)==SwarmModel::Llinf || net_.nodeState(*nit4)==SwarmModel::Luninf)
										LlLNegh4 ++;
									LlNegh4 ++;
									TripleNetwork::NeighborIteratorRange neghiters5 = net_.neighbors(*nit4);
									for(TripleNetwork::NeighborIterator nit5 = neghiters5.first; nit5 != neghiters5.second; nit5++)
									{
										if(*nit5 != *nit3)
										{
											if(net_.nodeState(*nit5)==SwarmModel::Lrinf || net_.nodeState(*nit5)==SwarmModel::Llinf || net_.nodeState(*nit5)==SwarmModel::Luninf)
												LlLNegh5 ++;
											LlNegh5 ++;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (LlNegh5 > 0)
			LlPower5 += LlLNegh5/LlNegh5;
		if (LlNegh4 > 0)
			LlPower4 += LlLNegh4/LlNegh4;
		if (LlNegh3 > 0)
			LlPower3 += LlLNegh3/LlNegh3;
		if (LlNegh2 > 0)
			LlPower2 += LlLNegh2/LlNegh2;
		if (LlNegh1 > 0)
			LlPower1 += LlLNegh1/LlNegh1;
	}
	stream() << sep << net_.numberOfNodes(SwarmModel::Llinf);
	stream() << sep << std::setprecision(5) << LlPower1/net_.numberOfNodes(SwarmModel::Llinf);
	stream() << sep << std::setprecision(5) << LlPower2/net_.numberOfNodes(SwarmModel::Llinf);
	stream() << sep << std::setprecision(5) << LlPower3/net_.numberOfNodes(SwarmModel::Llinf);
	stream() << sep << std::setprecision(5) << LlPower4/net_.numberOfNodes(SwarmModel::Llinf);
	stream() << sep << std::setprecision(5) << LlPower5/net_.numberOfNodes(SwarmModel::Llinf);

	stream() << '\n';
}
