/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkAntiAliasBinaryImageFilter.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Usage" << std::endl;
    std::cerr << argv[0] << "InputImageFile   labelValue" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef unsigned short                                      InputPixelType;
  typedef itk::Image< InputPixelType, Dimension >             InputImageType;

  typedef itk::ImageFileReader< InputImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }

  const InputPixelType labelValue = atoi( argv[2] );

  typedef unsigned char                                 BinaryPixelType;
  typedef double                                        RealPixelType;

  typedef itk::Image< BinaryPixelType, Dimension >      BinaryImageType;
  typedef itk::Image< RealPixelType, Dimension >        RealImageType;

  typedef itk::BinaryThresholdImageFilter< InputImageType, BinaryImageType >  ThresholdFilterType;

  ThresholdFilterType::Pointer binaryThresholdFilter = ThresholdFilterType::New();

  const BinaryPixelType outsideValue = 0;
  const BinaryPixelType insideValue = 255;

  binaryThresholdFilter->SetOutsideValue( outsideValue );
  binaryThresholdFilter->SetInsideValue(  insideValue  );

  const InputPixelType lowerThreshold = labelValue;
  const InputPixelType upperThreshold = labelValue;

  binaryThresholdFilter->SetLowerThreshold( lowerThreshold );
  binaryThresholdFilter->SetUpperThreshold( upperThreshold );

  binaryThresholdFilter->SetInput( reader->GetOutput() );

  typedef itk::AntiAliasBinaryImageFilter< BinaryImageType, RealImageType > AntiAliasFilterType;

  AntiAliasFilterType::Pointer antiAliasFilter = AntiAliasFilterType::New();

  antiAliasFilter->SetNumberOfIterations(50);

  antiAliasFilter->SetInput( binaryThresholdFilter->GetOutput() );


  typedef itk::ImageFileWriter< RealImageType >  WriterType;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( antiAliasFilter->GetOutput() );
  writer->SetFileName( argv[3] );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }

  return EXIT_SUCCESS;
}
