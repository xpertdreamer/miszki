package factorization

import (
	"reflect"
	"testing"
)

func TestTrialDivision(t *testing.T) {
	var tests = []struct {
		name string
		input uint
		expected []uint
	} {
		{
			name: "Error: number less than 2 (zero)",
			input: 0,
			expected: nil,
		},
		{
			name: "Prime number 2",
			input: 2,
			expected: []uint{2},
		},
		{
			name: "Prime number 13",
			input: 13,
			expected: []uint{13},
		},
		{
			name: "number 28",
			input: 28,
			expected: []uint{2, 2, 7},
		},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			ans, _ := TrialDivision(tt.input)
			if !reflect.DeepEqual(ans, tt.expected) {
				t.Errorf("got %v, expected %v", ans, tt.expected)
			}
		})
	}
}
