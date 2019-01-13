/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2014-2016. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "MonoImage.h"
/**
 * Included for ParticleAnalysisReport definition
 * TODO: Eliminate this dependency!
 */
#include "Vision/VisionAPI.h"

#include <vector>
#include <algorithm>

class BinaryImage : public MonoImage {
 public:
  virtual ~BinaryImage() = default;
  int GetNumberParticles();
  ParticleAnalysisReport GetParticleAnalysisReport(int particleNumber);
  void GetParticleAnalysisReport(int particleNumber,
                                 ParticleAnalysisReport *par);
  std::vector<ParticleAnalysisReport> *GetOrderedParticleAnalysisReports();
  BinaryImage *RemoveSmallObjects(bool connectivity8, int erosions);
  BinaryImage *RemoveLargeObjects(bool connectivity8, int erosions);
  BinaryImage *ConvexHull(bool connectivity8);
  BinaryImage *ParticleFilter(ParticleFilterCriteria2 *criteria,
                              int criteriaCount);
  virtual void Write(const char *fileName);

 private:
  bool ParticleMeasurement(int particleNumber, MeasurementType whatToMeasure,
                           int *result);
  bool ParticleMeasurement(int particleNumber, MeasurementType whatToMeasure,
                           double *result);
  static double NormalizeFromRange(double position, int range);
  static bool CompareParticleSizes(ParticleAnalysisReport particle1,
                                   ParticleAnalysisReport particle2);
};
