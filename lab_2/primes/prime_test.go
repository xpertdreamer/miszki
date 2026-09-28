package primes

import "testing"

func TestSieve(t *testing.T) {
	var tests = []struct {
		name string
		input uint
		expected bool
	} {
		{
			name: "Number less than 2 (zero)",
			input: 0,
			expected: false,
		},
		{
			name: "Prime number 2",
			input: 2,
			expected: true,
		},
		{
			name: "Prime number 13",
			input: 13,
			expected: true,
		},
		{
			name: "Number 28",
			input: 28,
			expected: false,
		},
		{
			name: "Number 10054",
			input: 10054,
			expected: false,
		},
		{
			name: "Number 19991",
			input: 19991,
			expected: true,
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			ans := SieveTest(tt.input)
			if ans != tt.expected {
				t.Errorf("got %v, expected %v", ans, tt.expected)
			}
		})
	}
}
