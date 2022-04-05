#ifndef DECODERS_H_
#define DECODERS_H_

#include "../Decoders/Decoder_MBT.hpp"
#include "../Decoders/Decoder_BP0.hpp"
#include "../Decoders/Decoder_BPI0.hpp"
#include "../Decoders/Decoder_FRFS0.hpp"
#include "../Decoders/Decoder_FRFSI0.hpp"

#define DecoderS(decoder,args) \
if( args.typeDecoder == "BP0" ) decoder = new Decoder_BP0(args.populationElite,args.populationMutants, args.rhoe);\
else if (args.typeDecoder == "FRFS0") decoder = new Decoder_FRFS0(args.populationElite,args.populationMutants,args.rhoe);\
else if (args.typeDecoder == "BPI0") decoder = new Decoder_BPI0(args.populationElite,args.populationMutants, args.rhoe);\
else if (args.typeDecoder == "FRFSI0") decoder = new Decoder_FRFSI0(args.populationElite,args.populationMutants,args.rhoe);\
else { cerr << "Unknown type" << endl; return 1; }

#define DecoderM(decoders,i,args) \
if( args.typeDecoders[i] == "BP0" ) decoders.push_back( new Decoder_BP0(args.configs[i][0],args.configs[i][1],args.configs[i][2]));\
else if (args.typeDecoders[i] == "FRFS0") decoders.push_back(new Decoder_FRFS0(args.configs[i][0],args.configs[i][1],args.configs[i][2]));\
else if (args.typeDecoders[i] == "BPI0") decoders.push_back(new Decoder_BPI0(args.configs[i][0],args.configs[i][1],args.configs[i][2]));\
else if (args.typeDecoders[i] == "FRFSI0") decoders.push_back(new Decoder_FRFSI0(args.configs[i][0],args.configs[i][1],args.configs[i][2]));\
else { cerr << "Unknown type" << endl; return 1; }

#endif /* DECODERS_H_ */
