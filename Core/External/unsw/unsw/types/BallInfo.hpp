#ifndef BALL_INFO_HPP
#define BALL_INFO_HPP

#include "types/Point.hpp"
#include "types/RRCoord.hpp"
#include "types/XYZ_Coord.hpp"

#include <iostream>
#include <cmath>


struct BallInfo {
   BallInfo () : visionVar(-1) {}
   BallInfo (RRCoord rr, int radius, Point imageCoords, XYZ_Coord neckRelative) :
      rr(rr),
      radius(radius),
      imageCoords(imageCoords),
      neckRelative(neckRelative),
      visionVar(0.0f),
      darkestPoint(0),
      angleVar(0.0f),
      ccdRating(0),
      lastSeen(0),
      lifetime(0){}

   virtual ~BallInfo () {}

   RRCoord rr;
   int radius;
   Point imageCoords;
   XYZ_Coord neckRelative;
   bool topCamera;

   /* A measure of truth as provided by vision */
   float visionVar;

   /* The darkest point in the sub-fovea around the ball
      as a percentage of the darkest point in the whole image*/
   int darkestPoint;

   /* Variance of the number of angles throughout the ball */
   float angleVar;

   /* The number of valid points from the CCD */
   int ccdRating;

   /* Number of frames since this ball estimate was seen */
   int lastSeen;

   /* Length of life for this ball so far */
   int lifetime;

   bool operator== (const BallInfo &other) const
   {
      return rr           == other.rr
          && radius       == other.radius
          && imageCoords  == other.imageCoords
          && visionVar    == other.visionVar
          && neckRelative == other.neckRelative
          && topCamera    == other.topCamera
          && lastSeen     == other.lastSeen
          && lifetime     == other.lifetime;
   }

   template<class Archive>
   void serialize(Archive &ar, const unsigned int file_version)
   {
      ar & rr;
      ar & radius;
      ar & imageCoords;
      ar & neckRelative;
      ar & topCamera;

      if (file_version >= 1) {
         ar & visionVar;
      } else {
         visionVar = 0.0;
      }

      ar & lastSeen;
      ar & lifetime;
   }
};

inline std::ostream& operator<<(std::ostream& os, const BallInfo& ballInfo) {
   os << ballInfo.rr;
   os << ballInfo.imageCoords;
   os.write((char*) &(ballInfo.radius), sizeof(int));
   os.write((char*) &(ballInfo.neckRelative), sizeof(XYZ_Coord));
   os.write((char*) &(ballInfo.visionVar), sizeof(float));
   os.write((char*) &(ballInfo.lastSeen), sizeof(int));
   os.write((char*) &(ballInfo.lifetime), sizeof(int));
   return os;
}

inline std::istream& operator>>(std::istream& is, BallInfo& ballInfo) {
   is >> ballInfo.rr;
   is >> ballInfo.imageCoords;
   is.read((char*) &(ballInfo.radius), sizeof(int));
   is.read((char*) &(ballInfo.neckRelative), sizeof(XYZ_Coord));
   is.read((char*) &(ballInfo.visionVar), sizeof(float));
   is.read((char*) &(ballInfo.lastSeen), sizeof(int));
   is.read((char*) &(ballInfo.lifetime), sizeof(int));
   return is;
}

struct BallHint
{

   enum Type
   {
      bLeft           = 0x00,
      bRight          = 0x01,
      bHidden         = 0x02,
      bNone           = 0x03
   };

   /* Names of corresponding enums from above */
   static const char *const TypeName[];

   BallHint () :
      type(bNone) {
   }

   BallHint (Type type) :
      type(type) {
   }

   virtual ~BallHint () {
   }

   Type type;

   template<class Archive>
   void serialize(Archive &ar, const unsigned int file_version) {
      ar & type;
   }

};

#ifndef SWIG
BOOST_CLASS_VERSION(BallInfo, 1);
#endif

#endif
