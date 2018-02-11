#ifndef FILTER_H
#define FILTER_H

// #include <Debug.h>

/*
* Implements a simple linear recursive exponential filter.
* See: http://www.statistics.com/glossary&term_id=756 */
template <class T, class OUT> class ExponentialFilter {
  // Weight for new values, as a percentage ([0..100])
  T m_WeightNew;

  // Current filtered value.
  OUT m_Current;

public:
  ExponentialFilter(T WeightNew, OUT Initial)
      : m_WeightNew(WeightNew), m_Current(Initial) {}

  void Filter(OUT New) {

    m_Current = (m_WeightNew * New + (100 - m_WeightNew) * m_Current) / 100;
  }

  void SetWeight(T NewWeight) { m_WeightNew = NewWeight; }

  T GetWeight() const { return m_WeightNew; }

  OUT Current() const { return m_Current; }

  void SetCurrent(OUT NewValue) { m_Current = NewValue; }
};

// Specialization for floating point math.
template <> class ExponentialFilter<float, float> {
  float m_fWeightNew;
  float m_fCurrent;

public:
  ExponentialFilter(float fWeightNew, float fInitial)
      : m_fWeightNew(fWeightNew / 100.0), m_fCurrent(fInitial) {}

  void Filter(float fNew) {
    m_fCurrent = m_fWeightNew * fNew + (1.0 - m_fWeightNew) * m_fCurrent;
  }

  void SetWeight(float NewWeight) { m_fWeightNew = NewWeight / 100.0; }

  float GetWeight() const { return m_fWeightNew * 100.0; }

  float Current() const { return m_fCurrent; }

  void SetCurrent(float fNewValue) { m_fCurrent = fNewValue; }
};

// Specialization for floating point math.
template <> class ExponentialFilter<unsigned char, long> {
  unsigned char m_WeightNew;

  // Current filtered value.
  long m_Current;

public:
  ExponentialFilter(unsigned char WeightNew, long Initial)
      : m_WeightNew(WeightNew), m_Current(Initial) {}

  void Filter(long New) {

    m_Current = (long long)(m_WeightNew * New +
                            ((100 - m_WeightNew) * (long long)m_Current)) /
                100;
  }

  void SetWeight(unsigned char NewWeight) { m_WeightNew = NewWeight; }

  unsigned char GetWeight() const { return m_WeightNew; }

  long Current() const { return m_Current; }

  void SetCurrent(long NewValue) { m_Current = NewValue; }
};

#endif /* end of include guard: FILTER_H */
