/* Defines the network processes, selects one of them at each step, and realizes it
 */
#include "SwarmModel.h"
#include <largenet/myrng/myrngWELL.h>
#include <cmath>

using namespace lnet;

SwarmModel::SwarmModel(lnet::TripleNetwork& net, Params par) :
	net_(net), par_(par)
{
}

double SwarmModel::step(const double currentTime)
{
	/* At each time step one of the processes is selected according to their global rates and then realized
	 */

	const double N1 = 1.0 / net_.numberOfNodes();

	/* Link creation rates
	* Note that we must divide by the total number of nodes here, since par_.alpha
	* is the probability for a single, say, L node to try to link to another randomly selected one.
	* Such a random node is an R node with probability R/N, and only then is the link established.
	* Hence, the average rate of L-R link creation is par_.alpha * L * R/N.*/
	const double alr = par_.ad * (net_.numberOfNodes(Luninf)+ net_.numberOfNodes(Llinf)+ net_.numberOfNodes(Lrinf))*(net_.numberOfNodes(Runinf)+ net_.numberOfNodes(Rlinf)+ net_.numberOfNodes(Rrinf))* N1,
				   all = par_.ae * (net_.numberOfNodes(Luninf)*net_.numberOfNodes(Luninf)+ net_.numberOfNodes(Luninf)*net_.numberOfNodes(Llinf)+net_.numberOfNodes(Luninf)*net_.numberOfNodes(Lrinf)+net_.numberOfNodes(Llinf)*net_.numberOfNodes(Llinf)+net_.numberOfNodes(Llinf)*net_.numberOfNodes(Lrinf)+net_.numberOfNodes(Lrinf)*net_.numberOfNodes(Lrinf))* N1,
				   arr = par_.ae * (net_.numberOfNodes(Runinf)*net_.numberOfNodes(Runinf)+ net_.numberOfNodes(Runinf)*net_.numberOfNodes(Rlinf)+net_.numberOfNodes(Runinf)*net_.numberOfNodes(Rrinf)+net_.numberOfNodes(Rlinf)*net_.numberOfNodes(Rlinf)+net_.numberOfNodes(Rlinf)*net_.numberOfNodes(Rrinf)+net_.numberOfNodes(Rrinf)*net_.numberOfNodes(Rrinf))* N1;

	/* Link destruction rate
	 */
	const double dlr = par_.dd * (net_.numberOfLinks(LuninfRuninf)+net_.numberOfLinks(LuninfRlinf)+net_.numberOfLinks(LuninfRrinf)+net_.numberOfLinks(LlinfRuninf)+net_.numberOfLinks(LlinfRlinf)+net_.numberOfLinks(LlinfRrinf)+net_.numberOfLinks(LrinfRuninf)+net_.numberOfLinks(LrinfRlinf)+net_.numberOfLinks(LrinfRrinf)),
			dll = par_.de * (net_.numberOfLinks(LuninfLuninf)+net_.numberOfLinks(LuninfLlinf)+net_.numberOfLinks(LuninfLrinf)+net_.numberOfLinks(LlinfLlinf)+net_.numberOfLinks(LlinfLrinf)+net_.numberOfLinks(LrinfLrinf)),
			drr = par_.de * (net_.numberOfLinks(RuninfRuninf)+net_.numberOfLinks(RuninfRlinf)+net_.numberOfLinks(RuninfRrinf)+net_.numberOfLinks(RlinfRlinf)+net_.numberOfLinks(RlinfRrinf)+net_.numberOfLinks(RrinfRrinf));

	/* Uninformed switching
	 */
	const double qurl = par_.q * (net_.numberOfNodes(Runinf)+net_.numberOfNodes(Rrinf)),
				  qulr = par_.q	* (net_.numberOfNodes(Luninf)+net_.numberOfNodes(Llinf));

	/* Informed switching
	 */
	const double qirl = par_.QL * net_.numberOfNodes(Rlinf),
				  qilr = par_.QR * net_.numberOfNodes(Lrinf);

	/* Conviction (w2/c)
	 */
	const double w2lr = par_.w2 * (net_.numberOfLinks(LuninfRuninf)+net_.numberOfLinks(LuninfRlinf)+net_.numberOfLinks(LuninfRrinf)+net_.numberOfLinks(LlinfRuninf)+net_.numberOfLinks(LlinfRlinf)+net_.numberOfLinks(LlinfRrinf)+net_.numberOfLinks(LrinfRuninf)+net_.numberOfLinks(LrinfRlinf)+net_.numberOfLinks(LrinfRrinf));

	/* Persuasion (w3/p)
	 */
	const double w3rlr = par_.w3 * (net_.numberOfTriples(RuninfLuninfRuninf)+net_.numberOfTriples(RuninfLuninfRlinf)+net_.numberOfTriples(RuninfLuninfRrinf)+net_.numberOfTriples(RuninfLlinfRuninf)+net_.numberOfTriples(RuninfLlinfRlinf)+net_.numberOfTriples(RuninfLlinfRrinf)+net_.numberOfTriples(RuninfLrinfRuninf)+net_.numberOfTriples(RuninfLrinfRlinf)+net_.numberOfTriples(RuninfLrinfRrinf)+net_.numberOfTriples(RlinfLuninfRlinf)+net_.numberOfTriples(RlinfLuninfRrinf)+net_.numberOfTriples(RlinfLlinfRlinf)+net_.numberOfTriples(RlinfLlinfRrinf)+net_.numberOfTriples(RlinfLrinfRlinf)+net_.numberOfTriples(RlinfLrinfRrinf)+net_.numberOfTriples(RrinfLuninfRrinf)+net_.numberOfTriples(RrinfLlinfRrinf)+net_.numberOfTriples(RrinfLrinfRrinf)),
				  w3lrl = par_.w3 * (net_.numberOfTriples(LuninfRuninfLuninf)+net_.numberOfTriples(LuninfRuninfLlinf)+net_.numberOfTriples(LuninfRuninfLrinf)+net_.numberOfTriples(LuninfRlinfLuninf)+net_.numberOfTriples(LuninfRlinfLlinf)+net_.numberOfTriples(LuninfRlinfLrinf)+net_.numberOfTriples(LuninfRrinfLuninf)+net_.numberOfTriples(LuninfRrinfLlinf)+net_.numberOfTriples(LuninfRrinfLrinf)+net_.numberOfTriples(LlinfRuninfLlinf)+net_.numberOfTriples(LlinfRuninfLrinf)+net_.numberOfTriples(LlinfRlinfLlinf)+net_.numberOfTriples(LlinfRlinfLrinf)+net_.numberOfTriples(LlinfRrinfLlinf)+net_.numberOfTriples(LlinfRrinfLrinf)+net_.numberOfTriples(LrinfRuninfLrinf)+net_.numberOfTriples(LrinfRlinfLrinf)+net_.numberOfTriples(LrinfRrinfLrinf));

	/* total rate for normalization
	 */
	const double atot = alr + all + arr + dlr + dll + drr + qurl + qulr + qirl + qilr + w2lr + w3rlr + w3lrl;
	if (atot == 0)
		return 1000;

	/* random number generation for processes
	 *
	 */
	double u = 0;
	while (u == 0.0)
		u = rng.Uniform01();

	const double tau = -std::log(u) / atot;

	const double x = atot * rng.Uniform01();

	/* Link creation between opposite state nodes
	 */
	double s = alr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Runinf);
		const double Npos2 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Rlinf);
		const double Npos3 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Rrinf);
		const double Npos4 = net_.numberOfNodes(Llinf) * net_.numberOfNodes(Runinf);
		const double Npos5 = net_.numberOfNodes(Llinf) * net_.numberOfNodes(Rlinf);
		const double Npos6 = net_.numberOfNodes(Llinf) * net_.numberOfNodes(Rrinf);
		const double Npos7 = net_.numberOfNodes(Lrinf) * net_.numberOfNodes(Runinf);
		const double Npos8 = net_.numberOfNodes(Lrinf) * net_.numberOfNodes(Rlinf);
		const double Npos9 = net_.numberOfNodes(Lrinf) * net_.numberOfNodes(Rrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6+Npos7+Npos8+Npos9);
		double z = 0;

		if (0 < y  && y <= Npos1)
			createLink(Luninf, Runinf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			createLink(Luninf, Rlinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			createLink(Luninf, Rrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			createLink(Llinf, Runinf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			createLink(Llinf, Rlinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			createLink(Llinf, Rrinf);
		z+=Npos6;
		if (z < y && y <= z+Npos7)
			createLink(Lrinf, Runinf);
		z+=Npos7;
		if (z < y && y <= z+Npos8)
			createLink(Lrinf, Rlinf);
		z+=Npos8;
		if (z < y && y <= z+Npos9)
			createLink(Lrinf, Rrinf);
		return tau;
	}

	/* Link creation between left nodes
	 */
	s += all;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Luninf);
		const double Npos2 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Llinf);
		const double Npos3 = net_.numberOfNodes(Luninf) * net_.numberOfNodes(Lrinf);
		const double Npos4 = net_.numberOfNodes(Llinf) * net_.numberOfNodes(Llinf);
		const double Npos5 = net_.numberOfNodes(Llinf) * net_.numberOfNodes(Lrinf);
		const double Npos6 = net_.numberOfNodes(Lrinf) * net_.numberOfNodes(Lrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6);
		double z = 0;
		if (0 < y && y <= Npos1)
			createLink(Luninf, Luninf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			createLink(Luninf, Llinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			createLink(Luninf, Lrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			createLink(Llinf, Llinf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			createLink(Llinf, Lrinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			createLink(Lrinf, Lrinf);
		return tau;
	}

	/* Link creation between right nodes
	 */
	s += arr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfNodes(Runinf) * net_.numberOfNodes(Runinf);
		const double Npos2 = net_.numberOfNodes(Runinf) * net_.numberOfNodes(Rlinf);
		const double Npos3 = net_.numberOfNodes(Runinf) * net_.numberOfNodes(Rrinf);
		const double Npos4 = net_.numberOfNodes(Rlinf) * net_.numberOfNodes(Rlinf);
		const double Npos5 = net_.numberOfNodes(Rlinf) * net_.numberOfNodes(Rrinf);
		const double Npos6 = net_.numberOfNodes(Rrinf) * net_.numberOfNodes(Rrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6);
		double z = 0;
		if (0 < y && y <= Npos1)
			createLink(Runinf, Runinf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			createLink(Runinf, Rlinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			createLink(Runinf, Rrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			createLink(Rlinf, Rlinf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			createLink(Rlinf, Rrinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			createLink(Rrinf, Rrinf);
		return tau;
	}

	/* Link deletion between opposite state nodes
	 */
	s += dlr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfLinks(LuninfRuninf);
		const double Npos2 = net_.numberOfLinks(LuninfRlinf);
		const double Npos3 = net_.numberOfLinks(LuninfRrinf);
		const double Npos4 = net_.numberOfLinks(LlinfRuninf);
		const double Npos5 = net_.numberOfLinks(LlinfRlinf);
		const double Npos6 = net_.numberOfLinks(LlinfRrinf);
		const double Npos7 = net_.numberOfLinks(LrinfRuninf);
		const double Npos8 = net_.numberOfLinks(LrinfRlinf);
		const double Npos9 = net_.numberOfLinks(LrinfRrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6+Npos7+Npos8+Npos9);
		double z = 0;

		if (0 < y && y <= Npos1)
			destroyLink(LuninfRuninf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			destroyLink(LuninfRlinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			destroyLink(LuninfRrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			destroyLink(LlinfRuninf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			destroyLink(LlinfRlinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			destroyLink(LlinfRrinf);
		z+=Npos6;
		if (z < y && y <= z+Npos7)
			destroyLink(LrinfRuninf);
		z+=Npos7;
		if (z < y && y <= z+Npos8)
			destroyLink(LrinfRlinf);
		z+=Npos8;
		if (z < y && y <= z+Npos9)
			destroyLink(LrinfRrinf);
		return tau;
	}

	/* Link deletion between left nodes
	 */
	s += dll;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfLinks(LuninfLuninf);
		const double Npos2 = net_.numberOfLinks(LuninfLlinf);
		const double Npos3 = net_.numberOfLinks(LuninfLrinf);
		const double Npos4 = net_.numberOfLinks(LlinfLlinf);
		const double Npos5 = net_.numberOfLinks(LlinfLrinf);
		const double Npos6 = net_.numberOfLinks(LrinfLrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6);
		double z = 0;
		if (0 < y && y <= Npos1)
			destroyLink(LuninfLuninf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			destroyLink(LuninfLlinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			destroyLink(LuninfLrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			destroyLink(LlinfLlinf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			destroyLink(LlinfLrinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			destroyLink(LrinfLrinf);
		return tau;
	}

	/* Link deletion between right nodes
	 */
	s += drr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfLinks(RuninfRuninf);
		const double Npos2 = net_.numberOfLinks(RuninfRlinf);
		const double Npos3 = net_.numberOfLinks(RuninfRrinf);
		const double Npos4 = net_.numberOfLinks(RlinfRlinf);
		const double Npos5 = net_.numberOfLinks(RlinfRrinf);
		const double Npos6 = net_.numberOfLinks(RrinfRrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6);
		double z = 0;
		if (0 < y && y <= Npos1)
			destroyLink(RuninfRuninf);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			destroyLink(RuninfRlinf);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			destroyLink(RuninfRrinf);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			destroyLink(RlinfRlinf);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			destroyLink(RlinfRrinf);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			destroyLink(RrinfRrinf);
		return tau;
	}

	/* Informed switching for left informed
	 */
	s += qirl;
	if (s >= x)
	{
		switchState(Rlinf, currentTime);
		return tau;
	}

	/* Informed switching for right informed
	 */
	s += qilr;
	if (s >= x)
	{
		switchState(Lrinf, currentTime);
		return tau;
	}

	/* Uninformed switching from right to left
	 */
	s += qurl;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfNodes(Runinf);
		const double Npos2 = net_.numberOfNodes(Rrinf);
		if (rng.Chance(Npos1/(Npos1+Npos2)))
			switchState(Runinf, currentTime);
		else
			switchState(Rrinf, currentTime);
		return tau;
	}

	/* Uninformed switching from left to right
	 */
	s += qulr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfNodes(Luninf);
		const double Npos2 = net_.numberOfNodes(Llinf);
		if (rng.Chance(Npos1/(Npos1+Npos2)))
			switchState(Luninf, currentTime);
		else
			switchState(Llinf, currentTime);
		return tau;
	}


	/* Linear state adoption due to one opposite state neighbor
	 */
	s += w2lr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfLinks(LuninfRuninf);
		const double Npos2 = net_.numberOfLinks(LuninfRlinf);
		const double Npos3 = net_.numberOfLinks(LuninfRrinf);
		const double Npos4 = net_.numberOfLinks(LlinfRuninf);
		const double Npos5 = net_.numberOfLinks(LlinfRlinf);
		const double Npos6 = net_.numberOfLinks(LlinfRrinf);
		const double Npos7 = net_.numberOfLinks(LrinfRuninf);
		const double Npos8 = net_.numberOfLinks(LrinfRlinf);
		const double Npos9 = net_.numberOfLinks(LrinfRrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6+Npos7+Npos8+Npos9);
		double z = 0;

		if (0 < y  && y <= Npos1)
			conviction(LuninfRuninf, currentTime);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			conviction(LuninfRlinf, currentTime);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			conviction(LuninfRrinf, currentTime);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			conviction(LlinfRuninf, currentTime);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			conviction(LlinfRlinf, currentTime);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			conviction(LlinfRrinf, currentTime);
		z+=Npos6;
		if (z < y && y <= z+Npos7)
			conviction(LrinfRuninf, currentTime);
		z+=Npos7;
		if (z < y && y <= z+Npos8)
			conviction(LrinfRlinf, currentTime);
		z+=Npos8;
		if (z < y && y <= z+Npos9)
			conviction(LrinfRrinf, currentTime);
		return tau;
	}

	/* Nonlinear state adoption due to two opposite state neighbors for right nodes
	 */
	s += w3lrl;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfTriples(LuninfRuninfLuninf);
		const double Npos2 = net_.numberOfTriples(LuninfRuninfLlinf);
		const double Npos3 = net_.numberOfTriples(LuninfRuninfLrinf);
		const double Npos4 = net_.numberOfTriples(LuninfRlinfLuninf);
		const double Npos5 = net_.numberOfTriples(LuninfRlinfLlinf);
		const double Npos6 = net_.numberOfTriples(LuninfRlinfLrinf);
		const double Npos7 = net_.numberOfTriples(LuninfRrinfLuninf);
		const double Npos8 = net_.numberOfTriples(LuninfRrinfLlinf);
		const double Npos9 = net_.numberOfTriples(LuninfRrinfLrinf);
		const double Npos10 = net_.numberOfTriples(LlinfRuninfLlinf);
		const double Npos11 = net_.numberOfTriples(LlinfRuninfLrinf);
		const double Npos12 = net_.numberOfTriples(LlinfRlinfLlinf);
		const double Npos13 = net_.numberOfTriples(LlinfRlinfLrinf);
		const double Npos14 = net_.numberOfTriples(LlinfRrinfLlinf);
		const double Npos15 = net_.numberOfTriples(LlinfRrinfLrinf);
		const double Npos16 = net_.numberOfTriples(LrinfRuninfLrinf);
		const double Npos17 = net_.numberOfTriples(LrinfRlinfLrinf);
		const double Npos18 = net_.numberOfTriples(LrinfRrinfLrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6+Npos7+Npos8+Npos9+Npos10+Npos11+Npos12+Npos13+Npos14+Npos15+Npos16+Npos17+Npos18);
		double z = 0;

		if (0 < y  && y <= Npos1)
			persuasion(LuninfRuninfLuninf, currentTime);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			persuasion(LuninfRuninfLlinf, currentTime);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			persuasion(LuninfRuninfLrinf, currentTime);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			persuasion(LuninfRlinfLuninf, currentTime);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			persuasion(LuninfRlinfLlinf, currentTime);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			persuasion(LuninfRlinfLrinf, currentTime);
		z+=Npos6;
		if (z < y && y <= z+Npos7)
			persuasion(LuninfRrinfLuninf, currentTime);
		z+=Npos7;
		if (z < y && y <= z+Npos8)
			persuasion(LuninfRrinfLlinf, currentTime);
		z+=Npos8;
		if (z < y && y <= z+Npos9)
			persuasion(LuninfRrinfLrinf, currentTime);
		z+=Npos9;
		if (z < y && y <= z+Npos10)
			persuasion(LlinfRuninfLlinf, currentTime);
		z+=Npos10;
		if (z < y && y <= z+Npos11)
			persuasion(LlinfRuninfLrinf, currentTime);
		z+=Npos11;
		if (z < y && y <= z+Npos12)
			persuasion(LlinfRlinfLlinf, currentTime);
		z+=Npos12;
		if (z < y && y <= z+Npos13)
			persuasion(LlinfRlinfLrinf, currentTime);
		z+=Npos13;
		if (z < y && y <= z+Npos14)
			persuasion(LlinfRrinfLlinf, currentTime);
		z+=Npos14;
		if (z < y && y <= z+Npos15)
			persuasion(LlinfRrinfLrinf, currentTime);
		z+=Npos15;
		if (z < y && y <= z+Npos16)
			persuasion(LrinfRuninfLrinf, currentTime);
		z+=Npos16;
		if (z < y && y <= z+Npos17)
			persuasion(LrinfRlinfLrinf, currentTime);
		z+=Npos17;
		if (z < y && y <= z+Npos18)
			persuasion(LrinfRrinfLrinf, currentTime);
		return tau;
	}

	/* Nonlinear state adoption due to two opposite state neighbors for left nodes
	 */
	s += w3rlr;
	if (s >= x)
	{
		const double Npos1 = net_.numberOfTriples(RuninfLuninfRuninf);
		const double Npos2 = net_.numberOfTriples(RuninfLuninfRlinf);
		const double Npos3 = net_.numberOfTriples(RuninfLuninfRrinf);
		const double Npos4 = net_.numberOfTriples(RuninfLlinfRuninf);
		const double Npos5 = net_.numberOfTriples(RuninfLlinfRlinf);
		const double Npos6 = net_.numberOfTriples(RuninfLlinfRrinf);
		const double Npos7 = net_.numberOfTriples(RuninfLrinfRuninf);
		const double Npos8 = net_.numberOfTriples(RuninfLrinfRlinf);
		const double Npos9 = net_.numberOfTriples(RuninfLrinfRrinf);
		const double Npos10 = net_.numberOfTriples(RlinfLuninfRlinf);
		const double Npos11 = net_.numberOfTriples(RlinfLuninfRrinf);
		const double Npos12 = net_.numberOfTriples(RlinfLlinfRlinf);
		const double Npos13 = net_.numberOfTriples(RlinfLlinfRrinf);
		const double Npos14 = net_.numberOfTriples(RlinfLrinfRlinf);
		const double Npos15 = net_.numberOfTriples(RlinfLrinfRrinf);
		const double Npos16 = net_.numberOfTriples(RrinfLuninfRrinf);
		const double Npos17 = net_.numberOfTriples(RrinfLlinfRrinf);
		const double Npos18 = net_.numberOfTriples(RrinfLrinfRrinf);

		const double y = rng.Uniform01()*(Npos1+Npos2+Npos3+Npos4+Npos5+Npos6+Npos7+Npos8+Npos9+Npos10+Npos11+Npos12+Npos13+Npos14+Npos15+Npos16+Npos17+Npos18);
		double z = 0;

		if (0 < y  && y <= Npos1)
			persuasion(RuninfLuninfRuninf, currentTime);
		z+=Npos1;
		if (z < y && y <= z+Npos2)
			persuasion(RuninfLuninfRlinf, currentTime);
		z+=Npos2;
		if (z < y && y <= z+Npos3)
			persuasion(RuninfLuninfRrinf, currentTime);
		z+=Npos3;
		if (z < y && y <= z+Npos4)
			persuasion(RuninfLlinfRuninf, currentTime);
		z+=Npos4;
		if (z < y && y <= z+Npos5)
			persuasion(RuninfLlinfRlinf, currentTime);
		z+=Npos5;
		if (z < y && y <= z+Npos6)
			persuasion(RuninfLlinfRrinf, currentTime);
		z+=Npos6;
		if (z < y && y <= z+Npos7)
			persuasion(RuninfLrinfRuninf, currentTime);
		z+=Npos7;
		if (z < y && y <= z+Npos8)
			persuasion(RuninfLrinfRlinf, currentTime);
		z+=Npos8;
		if (z < y && y <= z+Npos9)
			persuasion(RuninfLrinfRrinf, currentTime);
		z+=Npos9;
		if (z < y && y <= z+Npos10)
			persuasion(RlinfLuninfRlinf, currentTime);
		z+=Npos10;
		if (z < y && y <= z+Npos11)
			persuasion(RlinfLuninfRrinf, currentTime);
		z+=Npos11;
		if (z < y && y <= z+Npos12)
			persuasion(RlinfLlinfRlinf, currentTime);
		z+=Npos12;
		if (z < y && y <= z+Npos13)
			persuasion(RlinfLlinfRrinf, currentTime);
		z+=Npos13;
		if (z < y && y <= z+Npos14)
			persuasion(RlinfLrinfRlinf, currentTime);
		z+=Npos14;
		if (z < y && y <= z+Npos15)
			persuasion(RlinfLrinfRrinf, currentTime);
		z+=Npos15;
		if (z < y && y <= z+Npos16)
			persuasion(RrinfLuninfRrinf, currentTime);
		z+=Npos16;
		if (z < y && y <= z+Npos17)
			persuasion(RrinfLlinfRrinf, currentTime);
		z+=Npos17;
		if (z < y && y <= z+Npos18)
			persuasion(RrinfLrinfRrinf, currentTime);
		return tau;
	}

	return tau;
}

void SwarmModel::createLink(NodeState a, NodeState b)
{
	/* Creates a link between a random a node and a random b node
	 */
	if ((a == b) && (net_.numberOfNodes(a) <= 1))
		return;

	const LinkState s =
			static_cast<LinkState> (net_.linkStateCalculator()(a, b));
	const id_size_t nl = net_.numberOfLinks(s);

	const id_size_t max = (a == b) ? net_.numberOfNodes(a)
			* (net_.numberOfNodes(a) - 1) / 2 : net_.numberOfNodes(a)
			* net_.numberOfNodes(b);

	if (nl >= max)
		return;

	std::pair<bool, node_id_t> source, target;
	while (true)
	{
		source = net_.randomNode(a);
		if (!source.first)
			return;
		target = net_.randomNode(b);
		if (!target.first)
			return;
		if (source.second == target.second)
			continue;
		net_.addLink(source.second, target.second);
		if (net_.numberOfLinks(s) > nl)
			break;
	}
}

void SwarmModel::destroyLink(const LinkState s)
{
	/* Deletes a random link of state s
	 */
	std::pair<bool, link_id_t> l = net_.randomLink(s);
	if (l.first)
		net_.removeLink(l.second);
}

void SwarmModel::conviction(const LinkState s, const double tm)
{
	/* In the conviction event, selects a random link of state s
	 * The side to be convinced is selected randomly
	 */
	const std::pair<bool, link_id_t> l = net_.randomLink(s);
	if (!l.first)
		return;
	const double maj = static_cast <double>(net_.numberOfNodes(Rlinf)+net_.numberOfNodes(Rrinf)+net_.numberOfNodes(Runinf))/static_cast <double>(net_.numberOfNodes());
	const node_id_t n = rng.Chance(0.5) ? net_.source(l.second) : net_.target(l.second);
	if (net_.nodeState(n) == Luninf)
		net_.setNodeState(n, Runinf);
	else
	{
		if (net_.nodeState(n) == Llinf)
			net_.setNodeState(n, Rlinf);
		else
		{
			if (net_.nodeState(n) == Lrinf)
				net_.setNodeState(n, Rrinf);
			else
			{
				if (net_.nodeState(n) == Runinf)
					net_.setNodeState(n, Luninf);
				else
				{
					if (net_.nodeState(n) == Rlinf)
						net_.setNodeState(n, Llinf);
					else
						net_.setNodeState(n, Lrinf);
				}
			}
		}
	}
	bool inf = 1;
	if(s == LuninfRuninf || (s == LuninfRlinf && net_.nodeState(n) == Llinf) || (s == LuninfRrinf && net_.nodeState(n) == Lrinf) || (s == LlinfRuninf && net_.nodeState(n) == Rlinf) || (s == LrinfRuninf && net_.nodeState(n) == Rrinf) )
	{
		inf = 0;
	}
	alignEvents_.push_back(StateAlignEvent(tm, net_.nodeState(n), maj, inf));
}

void SwarmModel::persuasion(const TripleState s, const double tm)
{
	/* In the persuasion event, selects a random triplet of state s
	 * The middle node is persuaded
	 */
	const std::pair<bool, triple_id_t> t = net_.randomTriple(s);
	if (!t.first)
		return;
	const node_id_t n = net_.centerNode(t.second);
	const double maj = static_cast <double>(net_.numberOfNodes(Rlinf)+net_.numberOfNodes(Rrinf)+net_.numberOfNodes(Runinf))/static_cast <double>(net_.numberOfNodes());
	if (net_.nodeState(n) == Luninf)
		net_.setNodeState(n, Runinf);
	else
	{
		if (net_.nodeState(n) == Llinf)
			net_.setNodeState(n, Rlinf);
		else
		{
			if (net_.nodeState(n) == Lrinf)
				net_.setNodeState(n, Rrinf);
			else
			{
				if (net_.nodeState(n) == Runinf)
					net_.setNodeState(n, Luninf);
				else
				{
					if (net_.nodeState(n) == Rlinf)
						net_.setNodeState(n, Llinf);
					else
						net_.setNodeState(n, Lrinf);
				}
			}
		}
	}
	bool inf = 1;
	if(s == LuninfRuninfLuninf || s == LuninfRlinfLuninf || s == LuninfRrinfLuninf || s == RuninfLuninfRuninf || s == RuninfLlinfRuninf || s == RuninfLrinfRuninf )
	{
		inf = 0;
	}
	alignEvents_.push_back(StateAlignEvent(tm, net_.nodeState(n), maj, inf));
}

void SwarmModel::switchState(const NodeState s, const double t)
{
	/* A random node of state s is selected and switches from its current direction to the opposite direction
	 * Both informed and uninformed switching implemented here
	 */
	const std::pair<bool, node_id_t> a = net_.randomNode(s);
	if (!a.first)
		return;
	const node_id_t n = a.second;
	const double maj = static_cast <double>(net_.numberOfNodes(Rlinf)+net_.numberOfNodes(Rrinf)+net_.numberOfNodes(Runinf))/static_cast <double>(net_.numberOfNodes());
	if (net_.nodeState(n) == Luninf)
		net_.setNodeState(n, Runinf);
	else
	{
		if (net_.nodeState(n) == Llinf)
			net_.setNodeState(n, Rlinf);
		else
		{
			if (net_.nodeState(n) == Lrinf)
				net_.setNodeState(n, Rrinf);
			else
			{
				if (net_.nodeState(n) == Runinf)
					net_.setNodeState(n, Luninf);
				else
				{
					if (net_.nodeState(n) == Rlinf)
						net_.setNodeState(n, Llinf);
					else
						net_.setNodeState(n, Lrinf);
				}
			}
		}
	}
	switchEvents_.push_back(StateSwitchEvent(t, net_.nodeState(n),maj));
}
